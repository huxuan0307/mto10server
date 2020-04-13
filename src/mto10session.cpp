#include <mto10session.h>
#include <mto10server.h>
const char crlf[3]="\r\n";
static const char alpha[17]{"0123456789abcdef"};

int Mto10Session::HandleRequest(string&s)
{
    // Debug()<<"HandleRequest begin";
    static timeval tval;
    gettimeofday(&tval, nullptr);
    if ((tval.tv_sec << 10) + (tval.tv_usec >> 10) - (lastresponsetime_.tv_sec << 10) - (lastresponsetime_.tv_usec >> 10) > delay_)
    {
        // cout<<(tval.tv_sec << 10) + (tval.tv_usec >> 10) - (lastresponsetime_.tv_sec << 10) - (lastresponsetime_.tv_usec >> 10)<<endl;
        // cout<<delay_<<endl<<endl;
        timeout_ = true;
        return -1;
    }
    string msg;
    map<string,string> params;
    msg.swap(s);
    // cout << msg << endl;
    string key, val;
    char eq;
    stringstream ss(msg);
    while(ss>>key){
        ss>>eq>>val;
        params.emplace(key, val);
    }
    // for(auto&p:params){
    //     cout<<p.first<<'='<<p.second<<endl;
    // }
    if(params["Type"]=="ParameterAuthenticate"){
        // 参数确认
        auto str = DecodeMD5(params["MD5"]);
        Debug() << "user and pw: " << str;
        if(str[7]=='*'){
            // base
            mode_[0] = 's';
            stuno_ = str.substr(0, 7);
        }else if(str[7]=='-'){
            // competition
            mode_[0] = 'g';
            stuno_ = str.substr(0, 7);
            reverse(stuno_.begin(), stuno_.end());
        }else{
            shouldclose_ = true;
            return -1;
        }
        string pw = str.substr(8, 32);
        // TODO: 查询数据库&反馈结果
        login_res_ = server_ptr_->db_.check_login(stuno_, pw, string(1,*mode_));
        if(mode_[0]=='s'){
            // base
            login_suc_ = !!login_res_;
        }else if(mode_[0]=='g'){
            // competition
            login_suc_ = login_res_ > 0 && login_res_ <= 64;
        }
        if(login_suc_){
            Info() << "user" << stuno_ << "login successfully";
            if(params.count("Row") && params.count("Col")
                && params.count("GameID") && params.count("Delay")
                && params.count("Length")){
                // set parameters
                this->row_ = stoi(params["Row"]);
                this->col_ = stoi(params["Col"]);
                this->mapid_ = stoi(params["GameID"]);
                this->delay_ = stoi(params["Delay"]);
                // if(mode_[0] == 'g'){
                //     this->delay_ = 5000;
                // }
            }
            else{
                // 参数不合要求
                return -1;
            }
        }else{
            // 数据库禁止登录，gtest超过64，账号密码错误等多重原因
            string failedreason = (login_res_ == 0) ? "用户名或密码错误" : "已超过当前时段测试次数";
            Info()<<"user"<<stuno_ <<"login failed for "<< failedreason;
            return -1;
        }

    }else if(params["Type"]=="Coordinate"){
        if(params.count("Row")&&params.count("Col")&&params.count("Length")){
            coord[0] = params["Row"][0];
            coord[1] = params["Col"][0];
        }
        else{
            // 参数不合协议
            return -1;
        }
    }else{
        // Type不合协议
        return -1;
    }
    return 0;
}

void Mto10Session::Mto10Process(){
    if(!gamestart_){
        gettimeofday(&createtime_, nullptr);
        CreateNewGame();
        gamestart_ = true;
        response_ = mto10MessageToString(*this, "StartGame");
        return;
    }
    if(timeout_){
        // GameTimeout
        response_ = mto10MessageToString(*this, "GameTimeout" );
        gameovertype_ = GameOver_t::GameTimeout;
        gettimeofday(&completetime_, nullptr);
        shouldclose_ = true;
        return;
    }

    Info()<<"New merge at "<<coord[0]<<coord[1];

    GameMapMerge();
    UpdateGameData();
    if(mergecnt_>1){
        GameMapFallDown();
        GameMapRefill();
        CheckGameOver();
    }
    if(maxvalue_==16){
        // GameFinished
        response_ = mto10MessageToString(*this, "GameFinished");
        gameovertype_ = GameOver_t::GameFinished;
        shouldclose_ = true;
        return;
    }
    if(gameover_==true){
        // GameOver
        gettimeofday(&completetime_, nullptr);
        response_ = mto10MessageToString(*this, "GameOver");
        gameovertype_ = GameOver_t::GameOver;
        shouldclose_ = true;
        return;
    }
    
    if(mergecnt_ > 1){
        // merge succeeded
        response_ = mto10MessageToString(*this, "MergeSecceeded");
    }else if( mergecnt_ == 1 ){
        // merge failed
        response_ = mto10MessageToString(*this, "MergeFailed");
    }else{
        // InvalidCoordinate
        response_ = mto10MessageToString(*this, "InvalidCoordinate");
    }
    // Info().nospace() << "OldMap:\n" << showgamemap(oldgamemap_, row_, col_);
    // Info().nospace() << "NewMap:\n" << showgamemap(gamemap_, row_, col_);

}


void Mto10Session::CreateNewGame(){
    if(mode_[0] == 's'){
        // 纯随机
        if(mapid_==-1){
            mapid_ = rand();
        }
        if (row_ == -1)
            row_ = rand() % 4 + 5;
        if (col_ == -1)
            col_ = rand() % 6 + 5;
    }
    else if(mode_[0] == 'g'){
        // 使用随时间变化的有限的64个随机种子，取三个随机数作为mapid，row，col
        dre.seed(time(nullptr)/1800*64+login_res_);
        mapid_ = dre()&0x7fffffff;
        row_ = dre() % 4 + 5;
        col_ = dre() % 6 + 5;
    }
    gamemap_ = gamemap_t (row_, vector<int>(col_, 0));
    Info().nospace() << "New game started!"
                     << "\nGameID:" << mapid_
                     << "\nRow:" << row_ << " Col:" << col_
                     << "\nDelay:" << delay_;
    // 使用mapid_作为随机种子，保证相同的mapid游戏地图相同
    dre.seed(mapid_);
    
    queue<int> randnums = getRandomNums(3, row_*col_);
    for (int r = 0; r < row_; ++r){
        for (int c = 0; c < col_; ++c){
            this->gamemap_[r][c] = randnums.front();
            randnums.pop();
        }
    }
    Debug()<<"CreateNewGame complete";
}

void Mto10Session::GameMapMerge(){
    int coordrow;
    int coordcol;
    int score=0;
    if(isupper(coord[0])){
        coordrow = coord[0]-'A';
    }else if(islower(coord[0])){
        coordrow = coord[0]-'a';
    }else{
        // error
        mergecnt_ = 0;
        return ;
    }
    if(isdigit(coord[1])){
        coordcol = coord[1]-'0';
    }else{
        // error
        mergecnt_ = 0;
        return;
    }
    targetvalue_ = gamemap_[coordrow][coordcol];
    Debug().nospace() << "targetvalue = " << targetvalue_;

    if(coordrow>=0&&coordrow<row_ && coordcol>=0&&coordcol<col_){
        // 坐标合法
        gamemask_t mask = vector<vector<bool>>(this->row_, vector<bool>(this->col_, false));
        mergecnt_ = bfs(gamemap_, mask, {coordrow, coordcol});
        if(mergecnt_<=1)
            return;
        int value = gamemap_[coordrow][coordcol];
        // clean up
        for(int r=0;r<row_;++r){
            for(int c=0;c<col_;++c){
                if(mask[r][c]){
                    gamemap_[r][c] = 0;
                }
            }
        }
        assert(gamemap_[coordrow][coordcol] == 0);
        gamemap_[coordrow][coordcol] = value + 1;
        maxvalue_ = max(value+1, maxvalue_);
    }
    else{
        // error
        mergecnt_ = 0;
    }
    oldgamemap_ = gamemap_;
    return;
}

inline void Mto10Session::UpdateGameData(){
    if(mergecnt_>1){
        score_ += mergecnt_ * targetvalue_ * 3;
    }else if(mergecnt_ == 1){
        score_ -= 10;
    }else{
        score_ -= 50;
    }
    step_++;
}

void Mto10Session::GameMapFallDown(){
    // the row that block above can fall onto
    int fallplanerow;
    for (int c = 0; c < col_; ++c) {
        fallplanerow = row_;
        for (int r = row_ - 1; r >= 0; --r) {
            // find first 0
            if (gamemap_[r][c] != 0) {
                fallplanerow = r;
            }
            else {
                break;
            }
        }
        for (int r = fallplanerow - 1; r >= 0; --r) {
            if (gamemap_[r][c] != 0) {
                gamemap_[--fallplanerow][c] = gamemap_[r][c];
                gamemap_[r][c] = 0;
            }

        }
    } 
}

void Mto10Session::GameMapRefill(){
    queue<int> randnums = getRandomNums(maxvalue_, mergecnt_-1);
    for(int r=0;r<row_;++r){
        for(int c=0;c<col_;++c){
            if(gamemap_[r][c]==0){
                gamemap_[r][c] = randnums.front();
                randnums.pop();
            }
        }
    }
}

queue<int> Mto10Session::getRandomNums(int maxvalue, size_t cnt){
    string tmp;
    queue<int> res;
    if(oldmaxvalue_ != maxvalue){
        oldmaxvalue_ = maxvalue;
        switch (maxvalue)
        {
        case 4:
            distro_.param({0,3,3,3,1});
            break;
        case 5:
            distro_.param({0,25,25,25,15,10});
            break;
        case 6:
            distro_.param({0,20,20,20,20,15,5});
            break;
        case 7:
            distro_.param({0,20,20,20,20,10,5,5});
            break;
        case 8:
            distro_.param({0,16,16,16,16,16,10,5,5});
            break;
        case 9:
            distro_.param({0,13,13,13,13,13,13,10,5,5});
            break;
        case 10:
            distro_.param({0,11,11,11,11,11,11,11,10,5,5});
            break;
        case 11:
            distro_.param({0,10,10,10,10,10,10,10,10,10,5,5});
            break;
        case 12:
            distro_.param({0,9,9,9,9,9,9,9,9,9,10,5,5});
            break;
        case 13:
            distro_.param({0,8,8,8,8,8,8,8,8,8,8,10,5,5});
            break;
        case 14:
            distro_.param({0,7,7,7,7,7,7,7,7,7,7,7,10,5,5});
            break;
        case 15:
            distro_.param({0,7,7,7,7,7,7,7,7,7,7,7,7,10,5,5});
            break;
        default:
            break;
        }
    }
    for(size_t i{0}; i<cnt;++i){
        int val = distro_(dre);
        res.push(val);
        tmp.push_back(alpha[val]);
    }
    Debug() << "maxvalue:" << maxvalue_ << "random nums:" << tmp;
    return res;
}

void Mto10Session::CheckGameOver(){
    if(score_>0){
        gamemask_t mask = vector<vector<bool>>(this->row_, vector<bool>(this->col_, false));
        int cnt;
        for(int r=0;r<row_;++r){
            for(int c=0;c<col_;++c){
                cnt = bfs(gamemap_, mask, {r, c});
                if(cnt>1){
                    gameover_ = false;
                    return;
                }
            }
        }
        // nothing can be merge
        gameover_ = true;
        return;
    }
    else{
        // score_ <= 0
        gameover_ = true;
        return;
    }
}

string toString(const gamemap_t& gamemap, int row, int col){
    string res;
    for(int r=0;r<row;++r){
        for(int c=0;c<col;++c){
            res.push_back(alpha[gamemap[r][c]]);
        }
    }
    return res;
}

string mto10MessageToString(const Mto10Session&mto10, const string&Type, const string& Content)
{
    if(Type=="GameProgress"){
        return mto10MessageToString(mto10, Content);
    }else if(Type=="SecurityString"){
        stringstream ss;
        ss << "Type = SecurityString" << crlf
           << "Content = " << Content << crlf;
        ss << "Length = ";
        ss << ss.str().size() + 4 << crlf;
        return ss.str();
    }

}

string mto10MessageToString(const Mto10Session &mto10, const string &Content)
{
    stringstream ss;
    if(Content=="StartGame"){
        ss << "Type = GameProgress" << crlf
            << "Content = StartGame" << crlf
            << "Row = " << mto10.row_ << crlf
            << "Col = " << mto10.col_ << crlf
            << "GameID = " << mto10.mapid_ << crlf
            << "Delay = " << mto10.delay_ << crlf;
        ss << "Map = " << toString(mto10.gamemap_, mto10.row_, mto10.col_) << crlf;
    }
    else if(Content=="MergeSecceeded"){
        ss << "Type = GameProgress" << crlf
            << "Content = MergeSucceeded" << crlf
            << "GameID = " << mto10.mapid_ << crlf
            << "Step = " << mto10.step_ << crlf
            << "Score = " << mto10.score_<< crlf
            << "MaxValue = " << mto10.maxvalue_ << crlf;
        ss << "OldMap = " << toString(mto10.oldgamemap_, mto10.row_, mto10.col_) << crlf;
        ss << "NewMap = " << toString(mto10.gamemap_, mto10.row_, mto10.col_) << crlf;
    }
    else if(Content=="MergeFailed"){
        ss << "Type = GameProgress" << crlf
            << "Content = MergeFailed" << crlf
            << "GameID = " << mto10.mapid_ << crlf
            << "Step = " << mto10.step_ << crlf
            << "Score = " << mto10.score_ << crlf
            << "MaxValue = " << mto10.maxvalue_ << crlf;        
    }
    else if(Content=="InvalidCoordinate"){
        ss << "Type = GameProgress" << crlf
            << "Content = InvalidCoordinate" << crlf
            << "GameID = "  << mto10.mapid_ << crlf
            << "Step = "    << mto10.step_ << crlf
            << "Score = "   << mto10.score_ << crlf
            << "MaxValue = " << mto10.maxvalue_ << crlf; 
    }
    else if(Content=="GameOver"){
        ss << "Type = GameProgress" << crlf
            << "Content = GameOver" << crlf
            << "GameID = "      << mto10.mapid_ << crlf
            << "FinalStep = "   << mto10.step_ << crlf
            << "FinalScore = "  << mto10.score_ << crlf
            << "FinalMaxValue = " << mto10.maxvalue_ << crlf;
        ss << "FinalMap = " << toString(mto10.gamemap_, mto10.row_, mto10.col_) << crlf;
    }
    else if(Content=="GameFinished"){
        ss << "Type = GameProgress" << crlf
            << "Content = GameFinished" << crlf
            << "GameID = "      << mto10.mapid_ << crlf
            << "FinalStep = "   << mto10.step_ << crlf
            << "FinalScore = "  << mto10.score_ << crlf
            << "FinalMaxValue = " << mto10.maxvalue_ << crlf;
        ss << "FinalMap = " << toString(mto10.gamemap_, mto10.row_, mto10.col_) << crlf;
    }
    else if(Content=="GameTimeout"){
        ss << "Type = GameProgress" << crlf
            << "Content = GameTimeout" << crlf
            << "GameID = "      << mto10.mapid_ << crlf
            << "FinalStep = "   << mto10.step_ << crlf
            << "FinalScore = "  << mto10.score_ << crlf
            << "FinalMaxValue = " << mto10.maxvalue_ << crlf;
        ss << "FinalMap = " << toString(mto10.gamemap_, mto10.row_, mto10.col_) << crlf;
    }
    ss << "Length = ";
    ss << ss.str().size() + 5 << crlf;
    return ss.str();
}

string Mto10Session::DecodeMD5(const string&md5)
{
    Debug() << "md5:" << md5;
    Debug() << "sec:" << securityString_;
    string res(40, '\0');
    unsigned char ch;
    for(int i=0;i<40;++i){
        ch = isdigit(md5[i<<1]) ? (md5[i<<1] - '0') : (md5[i<<1] - 'a' + 10);
        ch = (ch<<4) + (isdigit(md5[(i<<1)|1]) ? (md5[(i<<1)|1] - '0') : (md5[(i<<1)|1] - 'a' + 10));
        res[i] = ch;
    }
    for(int i=0;i<40;++i){
        res[i]^=securityString_[i];
    }
    return res;
}

string showgamemap(gamemap_t const& gamemap, int row, int col)
{
    auto str = toString(gamemap, row, col);
    stringstream ss;
    ss<<"  |";
    for(int i=0;i<col;++i){
        ss<<"  "<<alpha[i];
    }
    ss << endl
       << "--+" << string(col * 3, '-') << endl;
    auto iter = str.begin();
    for(int r=0;r<row;++r){
        ss << char('A' + r) << " |";
        for(int c=0;c<col;++c){
            ss << setw(3) << *iter++;
        }
        ss << endl;
    }
    return ss.str();
}