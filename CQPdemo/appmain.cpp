/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载]
#include <string>
#include <regex>
#include <io.h>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <random>
#include <fstream>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

int fsize;
int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;
//vector<string> files;
map <int64_t, bool> game_status;
map <int64_t, int> game_round;
map <int64_t,map<int64_t,int>> score_board;
map <int64_t, int> song_num;
map <int64_t, int64_t> QQ_st;
int game_cnt;
vector<string> song_ori;
vector<string> song_name;
vector<int> song_len;
map <int64_t, int> rank_score;
map <int64_t, int> rank_high;
vector<int> rand_int;

void getFiles()
{
	song_ori.clear();
	song_name.clear();
	song_len.clear();
	rand_int.clear();
	int t;
	string tmp;
	ifstream fr("C:\\Users\\Administrator\\Desktop\\CQP\\data\\name.txt");
	ofstream fw("C:\\Users\\Administrator\\Desktop\\CQP\\data\\tst1.txt");
	while (getline(fr, tmp))
	{
		song_ori.push_back(tmp);
		song_name.push_back(regex_replace(tmp, regex("\\s\\(\\d\\)"), ""));
		fw << tmp << "\n";
	}
	fsize = song_name.size();
	fr.close();
	fw.close();
	fr.open("C:\\Users\\Administrator\\Desktop\\CQP\\data\\length.txt");
	fw.open("C:\\Users\\Administrator\\Desktop\\CQP\\data\\tst2.txt");
	for (int i = 0; i < fsize; i++)
	{
		fr >> t;
		song_len.push_back(t);
		rand_int.push_back(i);
		fw << song_name[i] << " " << song_len[i] << "\n";
	}
	fr.close();
	fw.close();
}

int days(int y, int m)
{
	if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) return 31;
	if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
	if (m == 2)
	{
		if ((y % 400 == 0) || (y % 4 == 0 && y % 100 != 0)) return 29;
		else return 28;
	}
}

string date_d(tm *now_time)
{
	ostringstream ss;
	ss << now_time->tm_year + 1900
		<< setfill('0') << setw(2) << now_time->tm_mon + 1
		<< setfill('0') << setw(2) << now_time->tm_mday;
	return ss.str();
}

string date_w(tm *now_time)
{
	ostringstream ss;
	if (now_time->tm_mday - now_time->tm_wday > 0)
	{
		ss << now_time->tm_year + 1900
			<< setfill('0') << setw(2) << now_time->tm_mon + 1
			<< setfill('0') << setw(2) << now_time->tm_mday - now_time->tm_wday;
	}
	else
	{
		if (now_time->tm_mon)
		{
			ss << now_time->tm_year + 1900
				<< setfill('0') << setw(2) << now_time->tm_mon
				<< setfill('0') << setw(2) << days(now_time->tm_year + 1900, now_time->tm_mon) + now_time->tm_mday - now_time->tm_wday;
		}
		else
		{
			ss << now_time->tm_year + 1900 - 1 << 12 << 31 + now_time->tm_mday - now_time->tm_wday;
		}
	}
	return ss.str();
}

string date_m(tm *now_time)
{
	ostringstream ss;
	ss << now_time->tm_year + 1900
		<< setfill('0') << setw(2) << now_time->tm_mon + 1;
	return ss.str();
}

void read_score(int num)
{
	int64_t QQ;
	int Score;
	rank_score.clear();
	rank_high.clear();
	time_t rawtime;
	time(&rawtime);
	tm *now_time = localtime(&rawtime);
	//printf("%d %d %d", now_time->tm_year + 1900, now_time->tm_mon + 1, now_time->tm_mday);
	ifstream ifs;
	if (num == 1) ifs.open("C:\\Users\\Administrator\\Desktop\\CQP\\data\\score\\guesssong\\day" + date_d(now_time));
	if (num == 2) ifs.open("C:\\Users\\Administrator\\Desktop\\CQP\\data\\score\\guesssong\\week" + date_w(now_time));
	if (num == 3) ifs.open("C:\\Users\\Administrator\\Desktop\\CQP\\data\\score\\guesssong\\mon" + date_m(now_time));
	while (ifs >> QQ >> Score)
	{
		rank_score[QQ] += Score;
		rank_high[QQ] = max(rank_high[QQ], Score);
	}
	ifs.close();
}

bool cmp(pair<int64_t, int> p1, pair<int64_t, int> p2)
{
	return p1.second > p2.second;
}

void show_score(int num, int64_t Group)
{
	time_t rawtime;
	time(&rawtime);
	tm *now_time = localtime(&rawtime);
	read_score(num);
	map <int64_t, int>::iterator it;
	ostringstream ss;
	int tmp;
	if (num == 1)
	{
		ss << "【猜歌日榜】 (" + date_d(now_time) + ")\n";
	}
	if (num == 2)
	{
		ss << "【猜歌周榜】 (" + date_w(now_time) + ")\n";
	}
	if (num == 3)
	{
		ss << "【猜歌月榜】 (" + date_m(now_time) + ")\n";
	}
	ss << "总分Top5：\n";
	vector <pair<int64_t, int>> Score;
	for (it = rank_score.begin(); it != rank_score.end(); it++)
	{
		Score.push_back(make_pair(it->first, it->second));
	}
	sort(Score.begin(), Score.end(), cmp);
	for (int i = 0; i < 5 || (i < Score.size() && Score[i].second && Score[i].second == Score[i - 1].second); i++)
	{
		if (i == 0 || (i < Score.size() && Score[i].second != Score[i - 1].second)) tmp = i + 1;
		ss << tmp << ". ";
		if (i < Score.size() && Score[i].second) ss << Score[i].first << " " << Score[i].second;
		ss << "\n";
	}
	ss << "单局Top5：\n";
	vector <pair<int64_t, int>> High;
	for (it = rank_high.begin(); it != rank_high.end(); it++)
	{
		High.push_back(make_pair(it->first, it->second));
	}
	sort(High.begin(), High.end(), cmp);
	for (int i = 0; i < 5 || (i < High.size() && High[i].second && High[i].second == High[i - 1].second); i++)
	{
		if (i == 0 || (i < High.size() && High[i].second != High[i - 1].second)) tmp = i + 1;
		ss << tmp << ". ";
		if (i < High.size() && High[i].second) ss << High[i].first << " " << High[i].second;
		ss << "\n";
	}
	if (num == 1)
	{
		ss << "数据每日重置";
	}
	if (num == 2)
	{
		ss << "数据每周日重置";
	}
	if (num == 3)
	{
		ss << "数据每月一日重置";
	}
	string s_tmp = ss.str();
	CQ_sendGroupMsg(ac, Group, s_tmp.c_str());
}

/* 
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
*/
CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	game_status.clear();
	game_round.clear();
	song_num.clear();
	score_board.clear();
	QQ_st.clear();
	getFiles();
	game_cnt = 0;
	return 0;
}


/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}

void AT(int64_t Group, int64_t QQ, string msg)
{
	ostringstream ss;
	ss << "[CQ:at,qq=" << QQ << "] " << msg;
	string s_tmp = ss.str();
	CQ_sendGroupMsg(ac, Group, s_tmp.c_str());
}


/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {
	if (!strcmp(msg, "songtst"))
	{
		stringstream ss;
		ss << fsize;
		string s_tmp = ss.str();
		CQ_sendPrivateMsg(ac, fromQQ, s_tmp.c_str());
		ss.str("");
		for (int i = 0; i < 5; i++)
		{
			ss << song_name[i] << " " << song_len[i] << "\n";
		}
		ss << song_name[fsize - 1] << " " << song_len[fsize - 1];
		s_tmp = ss.str();
		CQ_sendPrivateMsg(ac, fromQQ, s_tmp.c_str());
		//ss.str("");
		//如果要回复消息，请调用酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
		//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息
		return EVENT_IGNORE;
	}
}


/*
* Type=2 群消息
*/ 

int get_rand(int num)
{
	static mt19937 generator{ random_device()() };
	uniform_int_distribution<>distribution(0, num - 1);
	return distribution(generator);
}

int song_select(int len)
{
	srand((unsigned)time(0));
	random_shuffle(rand_int.begin(), rand_int.end());
	int rand_song = rand_int[get_rand(fsize)];
	int rand_st = get_rand(song_len[rand_song]-20) + 10;
	ostringstream ss;
	ss << "C:\\Users\\Administrator\\Desktop\\CQP\\bin\\ffmpeg.exe -i \"C:\\Users\\Administrator\\Desktop\\CQP\\musicpack\\" << song_ori[rand_song] << ".mp3\" -ss " 
		<< rand_st << " -c copy -t " << len << " \"" << "C:\\Users\\Administrator\\Desktop\\CQP\\data\\record\\" << song_name[rand_song] << ".mp3\" -y";
	string s_tmp = ss.str();
	system(s_tmp.c_str());//ffmpeg -i input.wmv -ss 30 -c copy -t 10 output.wmv
	//ss.str("");
	return rand_song;
	//CQ_sendGroupMsg(ac, fromGroup, ("[CQ:record,file=" + song_name(rand_song) + ".mp3]").c_str());
}

void send_ans(int64_t fromGroup)
{
	ostringstream ss;
	ss << "正确答案：" << song_name[song_num[fromGroup]];
	string s_tmp = ss.str();
	CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
	//ss.str("");
}

int get_score(int num)
{
	if (num >= 1 && num <= 3) return 1;
	if (num >= 4 && num <= 7) return 2;
	return 3;
}

void start_round(int64_t fromGroup,int num)
{
	game_round[fromGroup] = num;
	int song_len;
	if (num >= 1 && num <= 3) song_len = 5;
	else if (num >= 4 && num <= 7) song_len = 3;
	else song_len = 1;
	song_num[fromGroup] = song_select(song_len);
	ostringstream ss;
	ss << "Round " << num << "   分值 " << get_score(num);
	string s_tmp=ss.str();
	CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
	//ss.str("");
	s_tmp = "[CQ:record,file=" + song_name[song_num[fromGroup]] + ".mp3]";
	CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
	this_thread::sleep_for(chrono::duration<int>(30));
	if (game_round[fromGroup]==num)
	{
		game_round[fromGroup] = 0;
		CQ_sendGroupMsg(ac, fromGroup, "超时");
		send_ans(fromGroup); 
	}
}



CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	//if (!strcmp(msg,"[CQ:at,qq=3513312871] tst")) CQ_sendGroupMsg(ac,fromGroup,"[CQ:record,file=11-16 Loading ~Intro~.mp3]");
	if (fromQQ==490168650 && !strcmp(msg, "[CQ:at,qq=3513312871] 更新曲库"))
	{
		getFiles();
		CQ_sendGroupMsg(ac, fromGroup, "更新成功");
		return EVENT_IGNORE;
	}
	if (!strcmp(msg, "[CQ:at,qq=3513312871] 猜歌日榜"))
	{
		show_score(1, fromGroup);
		return EVENT_IGNORE;
	}
	if (!strcmp(msg, "[CQ:at,qq=3513312871] 猜歌周榜"))
	{
		show_score(2, fromGroup);
		return EVENT_IGNORE;
	}
	if (!strcmp(msg, "[CQ:at,qq=3513312871] 猜歌月榜"))
	{
		show_score(3, fromGroup);
		return EVENT_IGNORE;
	}
	if (fromQQ == 490168650 && !strcmp(msg, "[CQ:at,qq=3513312871] 终止猜歌"))
	{
		game_status[fromGroup] = false;
		return EVENT_IGNORE;
	}
	if (!strcmp(msg, "[CQ:at,qq=3513312871] 猜歌"))
	{
		if (game_status[fromGroup])
		{
			ostringstream ss;
			ss << "[CQ:at,qq=" << fromQQ << "] 正在游戏中";
			string s_tmp = ss.str();
			CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
			//ss.str("");
			return EVENT_IGNORE;
		}
		ostringstream ss;
		ss << "C:\\Users\\Administrator\\Desktop\\CQP\\data\\ingame\\" << fromGroup;
		ifstream ifs(ss.str());
		if (ifs)
		{
			AT(fromGroup, fromQQ, "当前群正在进行其他游戏");
			ifs.close();
			return EVENT_IGNORE;
		}
		/*else if (game_cnt >= 3)
		{
			ostringstream ss;
			ss << "[CQ:at,qq=" << fromQQ << "] 已经有3个群正在游戏中，请稍后再试";
			string s_tmp = ss.str();
			CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
			//ss.str("");
		}*/
		game_status[fromGroup] = true;
		ofstream ofs(ss.str());
		ofs.close();
		game_cnt++;
		QQ_st[fromGroup] = fromQQ;
		CQ_sendGroupMsg(ac, fromGroup, "规则：连续5个字符（3个汉字）匹配则正确，若少于5个字符则需全部匹配。");
		//CQ_sendGroupMsg(ac, fromGroup, "目前不支持选择游戏");
		this_thread::sleep_for(chrono::duration<int>(5));
		for (int i = 1; i <= 10; i++)
		{
			if (!game_status[fromGroup]) break;
			start_round(fromGroup, i);
			this_thread::sleep_for(chrono::duration<int>(5));
		}
		game_round[fromGroup] = 0;
		game_status[fromGroup] = false;
		QQ_st[fromGroup] = 0;
		game_cnt--;
		CQ_sendGroupMsg(ac, fromGroup, "游戏结束");
		ss.str("");
		map<int64_t, int>::iterator iter;
		time_t rawtime;
		time(&rawtime);
		tm *now_time = localtime(&rawtime);
		ofstream fd("C:\\Users\\Administrator\\Desktop\\CQP\\data\\score\\guesssong\\day" + date_d(now_time), ofstream::app);
		ofstream fw("C:\\Users\\Administrator\\Desktop\\CQP\\data\\score\\guesssong\\week" + date_w(now_time), ofstream::app);
		ofstream fm("C:\\Users\\Administrator\\Desktop\\CQP\\data\\score\\guesssong\\mon" + date_m(now_time), ofstream::app);
		ss<<"===计分板===\n";
		for (iter = score_board[fromGroup].begin(); iter != score_board[fromGroup].end(); iter++)
		//for (int i = 0; i < score_board[fromGroup].size; i++)
		{
			ss << "[CQ:at,qq=" << iter->first << "] " << iter->second<<"\n";
			fd << iter->first << " " << iter->second << "\n";
			fw << iter->first << " " << iter->second << "\n";
			fm << iter->first << " " << iter->second << "\n";
		}
		fd.close();
		fw.close();
		fm.close();
		ss<< "===========";
		string s_tmp = ss.str();
		CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
		score_board[fromGroup].clear();
		ss.str("");
		ss << "C:\\Users\\Administrator\\Desktop\\CQP\\data\\ingame\\" << fromGroup;
		s_tmp = ss.str();
		//CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
		remove(s_tmp.c_str());
		return EVENT_IGNORE;
	}
	if (game_status[fromGroup] && game_round[fromGroup])
	{
		int round_now = game_round[fromGroup];
		//if (regex_match(song_name(song_num), regex(msg, regex_constants::icase)) || (strlen(msg) >= 5 && regex_search(song_name(song_num), regex(msg,regex_constants::icase))))
		string song_name_l = song_name[song_num[fromGroup]],msg_l=msg;
		transform(song_name_l.begin(), song_name_l.end(), song_name_l.begin(), ::tolower);
		transform(msg_l.begin(), msg_l.end(), msg_l.begin(), ::tolower);
		//regex_replace(song_name_l, regex("\s\(\n\)"), "");
		//if (!game_round[fromGroup]) return EVENT_IGNORE;
		if (song_name_l==msg_l || (strlen(msg)>=5 && song_name_l.find(msg_l)!=string::npos))
		{
			if (game_round[fromGroup]!=round_now) return EVENT_IGNORE;
			score_board[fromGroup][fromQQ] += get_score(game_round[fromGroup]);
			game_round[fromGroup] = 0;
			ostringstream ss;
			ss << "[CQ:at,qq=" << fromQQ << "] 回答正确！";
			string s_tmp = ss.str();
			CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
			//CQ_sendGroupMsg(ac, fromGroup,ss.str().c_str());
			//ss.str("");
			send_ans(fromGroup);
			//game_status[fromGroup]=false;
		}
		return EVENT_IGNORE;
	}
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=4 讨论组消息
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=101 群事件-管理员变动
* subType 子类型，1/被取消管理员 2/被设置管理员
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=201 好友事件-好友已添加
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

/*
* 菜单，可在 .json 文件中设置菜单数目、函数名
* 如果不使用菜单，请在 .json 及此处删除无用菜单
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "这是menuA，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "这是menuB，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}
