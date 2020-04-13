#ifndef _MTO10SESSION_
#define _MTO10SESSION_
#include <string>
#include <fstream>
#include "logging.h"
#include <vector>
#include <sstream>
#include <regex>
#include <ctime>
#include <random>
#include <queue>
#include <cctype>
#include <map>
#include <Database.h>
#include <sys/time.h>
#include <atomic>
using namespace std;

extern const char crlf[3];


using gamemap_t = vector<vector<int>>;
using gamemask_t = vector<vector<bool>>;
class Mto10Session;
class Mto10Server;
string toString(const gamemap_t& gamemap, int row, int col);
string mto10MessageToString(const Mto10Session &mto10, const string &Content);
string mto10MessageToString(const Mto10Session&mto10, const string&Type, const string& Content);
string showgamemap(gamemap_t const& gamemap, int row, int col);
struct Mto10Coordinate{
    char Row;
    char Col;
};

struct Mto10GameParameter{
    int Row;
    int Col;
    int GameID;
    int Delay;
};

struct Mto10GameProgressStartGame{
    string Content;
    int Row;
    int Col;
    int GameID;
    int Delay;
    string Map;
    string toString(){
        stringstream ss;
        ss  <<"Type = GameProgress"     <<crlf
            <<"Content = StartGame"     <<crlf
            <<"Row = "<<Row             <<crlf
            <<"Col = "<<Col             <<crlf
            <<"GameID = "<<GameID       <<crlf
            <<"Delay = "<<Delay         <<crlf;
        ss  <<"Map = "<<Map             <<crlf;
        return ss.str();
    }
};

struct Mto10GameProgressMergeSucceeded{
    int GameID;
    int Step;
    int Score;
    int MaxValue;
    string OldMap;
    string NewMap;
    int mapsize;
    string toString(){
        stringstream ss;
        ss  <<"Type = GameProgress"         <<crlf
            <<"Content = MergeSucceeded"    <<crlf
            <<"GameID = "<<GameID           <<crlf
            <<"Step = "<<Step               <<crlf
            <<"Score = "<<Score             <<crlf
            <<"MaxValue = "<<MaxValue       <<crlf;
        ss  <<"OldMap = "<<OldMap           <<crlf;
        ss  <<"NewMap = "<<NewMap           <<crlf;
        return ss.str();
    }
};

struct Mto10GameProgressMergeFailed{
    int GameID;
    int Step;
    int Score;
    int MaxValue;
    string toString(){
        stringstream ss;
        ss  <<"Type = GameProgress"         <<crlf
            <<"Content = MergeFailed"       <<crlf
            <<"GameID = "<<GameID           <<crlf
            <<"Step = "<<Step               <<crlf
            <<"Score = "<<Score             <<crlf
            <<"MaxValue = "<<MaxValue       <<crlf;
        return ss.str();
    }
};

struct Mto10GameProgressInvalidCoordinate{
    int GameID;
    int Step;
    int Score;
    int MaxValue;
    string toString(){
        stringstream ss;
        ss  <<"Type = GameProgress"         <<crlf
            <<"Content = InvalidCoordinate" <<crlf
            <<"GameID = "<<GameID           <<crlf
            <<"Step = "<<Step               <<crlf
            <<"Score = "<<Score             <<crlf
            <<"MaxValue = "<<MaxValue       <<crlf;
        return ss.str();
    }
};


struct Mto10GameProgressGameOver{
    int GameID;
    int FinalStep;
    int FinalScore;
    int FinalMaxValue;
    string FinalMap;
    int mapsize;
    string toString(){
        stringstream ss;
        ss  <<"Type = GameProgress"         <<crlf
            <<"Content = GameOver"          <<crlf
            <<"GameID = "<<GameID           <<crlf
            <<"FinalStep = "<<FinalStep     <<crlf
            <<"FinalScore = "<<FinalScore   <<crlf
            <<"FinalMaxValue = "<<FinalMaxValue       <<crlf;
        ss  <<"FinalMap = "<<FinalMap       <<crlf;
        return ss.str();
    }
};

struct Mto10ParameterAuthenticate{
    string MD5;
    int Row;
    int Col;
    int GameID;
    int Delay;
};

struct Mto10SecurityContext{
    string Content;
    string toString(){
        stringstream ss;
        ss  <<"Type = SecurityString"   <<crlf
            <<"Content = "<<Content     <<crlf;
        return ss.str();
    }
};

class Mto10Session
{
private:
    /* data */
    int mapid_;
    gamemap_t gamemap_;
    gamemap_t oldgamemap_;
    int score_;
    int row_;
    int col_;
    int step_;
    int maxvalue_;
    int delay_;
    
    /*  */
    string md5_;
    default_random_engine dre;
    discrete_distribution<int> distro_;
    int oldmaxvalue_;
    // for a step
    /*coordinate coord[0]:row, coord[1]:col*/
    char coord[2];
    int targetvalue_;
    /*num of block in a merge*/
    int mergecnt_;
    string response_;
    
    bool gameover_;
    bool timeout_;

    // for login
    string stuno_;
    // for session control
    bool login_suc_;
    int login_res_;
    char mode_[1];      // 's':base, 'g':competition
    timeval createtime_;
    timeval completetime_;
    timeval lastresponsetime_;
    bool gamestart_;
    bool shouldclose_;
    string securityString_;
    GameOver_t gameovertype_;

    // server
    Mto10Server* server_ptr_;

    inline int64_t getCreatetime(){
        return createtime_.tv_sec * 1000 + createtime_.tv_usec / 1000;
    }
    inline int64_t getCompletetime(){
        return completetime_.tv_sec * 1000 + completetime_.tv_usec / 1000;
    }
public:
    Mto10Session(Mto10Server* server_ptr)
        : dre{}, shouldclose_{false}, gameover_{false}, timeout_{false}, gamestart_{false},
          maxvalue_{3}, oldmaxvalue_{3}, score_{100}, step_{0}, mode_{0}, login_suc_{false},
          distro_{{0,1,1,1}}, delay_{60000}, server_ptr_{server_ptr}
    {}
    ~Mto10Session() {
        // cout<<"~Mto10Session"<<endl;
    }
    int HandleRequest(string& s);
    void CreateNewGame();

    queue<int> getRandomNums(int maxvalue, size_t cnt);
    // 处理并生成反馈数据
    void Mto10Process();
    // 将数据放进服务端的发送缓冲区
    void AddToBuf(string&s){
        response_.swap(s);
    }
private:
    void GameMapMerge();
    void UpdateGameData();
    void GameMapFallDown();
    void GameMapRefill();
    void CheckGameOver();

    int bfs(gamemap_t const& gamemap, gamemask_t&mask, pair<int,int> point0){
        		queue<pair<int, int>> q;
		const auto& value = gamemap[point0.first][point0.second];
		if (mask[point0.first][point0.second] == 1)
			return 0;
		q.push (point0);
		set<pair<int, int>> s;
		while (!q.empty ()) {
			auto& r = q.front ().first;
			auto& c = q.front ().second;
			mask[r][c] = true;
			s.insert ({ r,c });
			// up
			if (r > 0 && !mask[r - 1][c] && gamemap[r - 1][c] == value) {
				q.emplace (r - 1, c);
			}
			// down
			if (r + 1 < row_ && !mask[r + 1][c] && gamemap[r + 1][c] == value) {
				q.emplace (r + 1, c);
			}
			// left
			if (c > 0 && !mask[r][c - 1] && gamemap[r][c - 1] == value) {
				q.emplace (r, c - 1);
			}
			// right
			if (c + 1 < col_ && !mask[r][c + 1] && gamemap[r][c + 1] == value) {
				q.emplace (r, c + 1);
			}
			q.pop ();
		}
		return (int)s.size ();
    }

    string DecodeMD5(const string&md5);

    friend string mto10MessageToString(const Mto10Session& mto10, const string&Content);
    friend class Mto10Server;
};




#endif