#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <md5.h>
#include "networkServer.h"
#include "argument.h"
#define STR(x) (#x)
using namespace std;
using namespace chrono;
//static vector<string> split(string, )

class TextMessage
{
	using Title = string;
	using Content = string;
public:
	vector<pair<Title, Content>> data;
	TextMessage () { }
	TextMessage (vector<char>&& str)
	{
		stringstream ss (string (begin (str), end (str)));
		string buf;
		Title title;
		string content;
		char eq;
		while (ss >> title >> eq >> content)
			data.push_back ({ title, content });
	}

	void showData ()
	{
#if _HAS_CXX17
		for (auto& [title, content] : data) {
			cout << title << " = " << content << endl;
		}
#else
		for (auto& p : data) {
			cout << p.first << " = " << p.second << endl;
		}
#endif //  __HAS_CXX17

	}
	string toString ()
	{
		stringstream ss;
#if _HAS_CXX17
		for (auto& [title, content] : data) {
			ss << title << " = " << content << "\r\n";
		}
#else
		for (auto& p : data) {
			ss << p.first << " = " << p.second << "\r\n";
		}

#endif
		return ss.str ();
	}
	string getVal (string const& title)
	{
#if _HAS_CXX17
		for (auto& [t, c] : data) {
			if (t == title)
				return c;
		}
#else
		for (auto& p : data) {
			if (p.first == title)
				return p.second;
		}

#endif
		return {};
	}
};

class MTO10
{
public:
	enum class MessageType
	{
		SecurityString,
		GameProgress,
	};
#if _HAS_CXX17
	using var = variant<int, uint16_t, string>;

	MTO10 (unordered_map<string, var>& params) :
		ipaddr{ get<2> (params["ipaddr"]) },
		port{ get<1> (params["port"]) },
		mapid{ get<0> (params["mapid"]) },
		network{ ipaddr, port },
		stuno{ get<2> (params["stuno"]) },
		passwd{ get<2> (params["passwd"]) },
		delay{ get<0> (params["delay"]) },
		row{ get<0> (params["row"]) },
		col{ get<0> (params["col"]) }
	{
		init ();
		run ();
	}
#endif
	MTO10 (unordered_map<string, string>& params) :
		ipaddr{ params["ipaddr"] },
		port{ static_cast<uint16_t>(stoul (params["port"])) },
		mapid{ static_cast<uint16_t>(stoul (params["mapid"])) },
		network{ ipaddr, port },
		stuno{ (params["stuno"]) },
		passwd{ (params["passwd"]) },
		delay{ static_cast<int> (stoul (params["delay"])) },
		row{ static_cast<int>(stoul (params["row"])) },
		col{ static_cast<int>(stoul (params["col"])) },
		score{ 0 }
	{
		Info()<<"MTO10 CTOR";
		init ();
		run ();

	}

	MTO10(mto10_param_t const& t):
		score{ 0 }, maxvalue{ 3 }
	{
		std::tie (ipaddr, port, mapid, stuno, passwd, row, col, delay, runmode) = t;
		network.init (ipaddr, port);
		network.start ();
		init ();
		run ();
	}


	void sendCoord (string&& coord)
	{
		TextMessage text;
		text.data.push_back ({ "Type","Coordinate" });
		text.data.push_back ({ "Row",move (coord.substr (0,1)) });
		text.data.push_back ({ "Col",move (coord.substr (1,1)) });
		text.data.push_back ({ "Length", move (to_string (50)) });
		network.writeToNet (text.toString ());
		Info () << "send coordinate: " << coord;
		cout << "coordinate: " << coord << endl;
	}

	void GameOver ()
	{
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j) {
				cout << rMessage["FinalMap"][i * col + j] << ' ';
			}
			cout << endl;
		}
		cout << endl;
	}

	void run ()
	{
		while (true) {
			sendCoord (solve{ gamemap, maxvalue, SolveType::Algo1 }());
			rMessage = getMessage ();

			if (rMessage["Type"] != "GameProgress") {
				Critical () << "in run() get Type = " << rMessage["Type"];
			}
			if (rMessage["Content"] == "GameOver") {
				Info () << "GameOver!!";
				break;
			}
			else if (rMessage["Content"] == "GameFinished") {
				Info () << "GameFinished!";
				break;
			}
			else if (rMessage["Content"] == "GameTimeout") {
				Info () << "GameTimeout!";
				break;
			}
			else if (rMessage["Content"] == "MergeFailed") {
				Info () << "MergeFailed";
				// Type
				// Content
				// GameID
				// Step
				// Score
				// MaxValue
				// Length
			}
			else if (rMessage["Content"] == "InvalidCoordinate") {
				Info () << "InvalidCoordinate";
			}
			else if (rMessage["Content"] == "MergeSucceeded") {
				setGameMap (rMessage["NewMap"]);
				setMaxValue (stoi (rMessage["MaxValue"]));
			}
			score = stoi (rMessage["Score"]);
			step = stoi (rMessage["Step"]);
			showGameMap ();
			cout << rMessage["Content"] << ", " << "score=" << score << endl;
			Info () << rMessage["Content"] << ", " << "score=" << score;
		}
		score = stoi (rMessage["FinalScore"]);
		step = stoi (rMessage["FinalStep"]);
		maxvalue = stoi (rMessage["FinalMaxValue"]);
		setGameMap (rMessage["FinalMap"]);
		showGameMap ();
		Info () << "FinalScore: " << score << "FinalStep: " << step << "FinalMaxValue: " << maxvalue;
		cout << "FinalScore: " << score << "FinalStep: " << step << "FinalMaxValue: " << maxvalue << endl;
		cout << "weight score: " << double (score) / row / col << endl;
	}

	void init ()
	{
		rMessage = getMessage (88);
		//auto str = getRandStr ();

		//auto randstr = TextMessage (move (str)).getVal ("Content");
		auto randstr = rMessage["Content"];
		MD5 md5 (passwd);
		string str1 = stuno + "*" + md5.toStr ();
		string res (81, 0);
		stringstream ss;
		for (int i = 0; i < 40; ++i) {
			char c = str1[i] ^ randstr[i];
			ss.fill ('0');
			ss << setw (2) << hex << static_cast<int>(c);
		}
		cout << ss.str () << endl;
		sendParameterAuthenticate (ss.str ());
		rMessage = getMessage (100);
		row = stoi (rMessage["Row"]);
		col = stoi (rMessage["Col"]);
		mapid = stoi (rMessage["GameID"]);
		gamemap = vector<vector<int>> (row, vector<int> (col, '0'));
#if _HAS_CXX17
		for (auto& [key, val] : rMessage) {
			cout << key << " = " << val << endl;
		}
#else
		for (auto& p : rMessage) {
			cout << p.first << " = " << p.second << endl;
		}
#endif
		setGameMap (rMessage["Map"]);
		showGameMap ();
	}

	unordered_map<string, string> getMessage (int len = 50)
	{
		unordered_map<string, string> m;
		while (network.getBufLen () < len)
			this_thread::sleep_for (milliseconds(25));
		while (network.getBufLen ()) {
			stringstream ss (network.getline ());
			string title, value;
			char eq;
			ss >> title >> eq >> value;
			m[title] = value;
		}
		return m;
	}

	void sendParameterAuthenticate (string&& md5)
	{
		string message;
		TextMessage text;
		text.data.push_back ({ "Type", "ParameterAuthenticate" });
		text.data.push_back ({ "MD5",move (md5) });
		text.data.push_back ({ "Row",to_string (row) });
		text.data.push_back ({ "Col",to_string (col) });
		text.data.push_back ({ "GameID",to_string (mapid) });
		text.data.push_back ({ "Delay",to_string (delay * 1000) });
		message = text.toString ();
		message += "Length = ";
		message += to_string (message.size () + 5);
		message += "\r\n";
		/*Debug () << message;*/
		network.writeToNet (move (message));
		//this_thread::sleep_for (1s);
	}

	void showGameMap ()
	{
		for (auto& v : gamemap) {
			copy (begin (v), end (v), ostream_iterator<char> (cout, " "));
			endl (cout);
		}
	}

	void setGameMap (string mapstring)
	{
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j) {
				gamemap[i][j] = mapstring[i * col + j] - '0';
			}
		}
	}

	void setMaxValue (int value)
	{
		maxvalue = value;
	}

	void setStep (int step)
	{
		this->step = step;
	}

private:
	int mapid;
	unordered_map<string, string> rMessage;
	vector<vector<int>> gamemap;
	int score;
	int row;
	int col;
	int step;
	int maxvalue;

	string ipaddr;
	uint16_t port;
	string stuno;
	string passwd;
	NetworkForMto10 network;
	int delay;
	int runmode;
};


// 2 3 2 3 2 3
// 3 1 2 2 1 3
// 3 1 2 2 1 3
// 3 2 2 3 3 3
// 3 3 1 3 3 3

// 0 0 0 3 2 3
// 2 0 3 1 3 1
// 1 3 1 3 2 2
// 3 3 3 3 1 3
// 3 3 1 2 1 1

// 0 0 0 3 2 3
// 2 0 3 1 3 1
// 1 1 3 1 3 2
// 2 2 3 3 3 3
// 3 3 1 3 3 3