/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����]
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
//#define SN(i) (regex_replace(regex_replace(regex_replace(files[i],regex("C:.Users.Administrator.Desktop.CQP.musicpack."),""),regex(".mp3"),""),regex("\\s\\(\\d\\)"),""))
//#define SN(i) regex_replace(song_name[i], regex("\\s\\(\\d\\)"), "")
//#define w2s(i) i
//#define cjj490168650 Administrator

using namespace std;

const int fsize = 1417;
int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;
const char * filePath = "C:\\Users\\Administrator\\Desktop\\CQP\\musicpack";
const char * filePath2 = "C:\\Users\\Administrator\\Desktop\\CQP\\data\\record";
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

void getFiles()
{
	song_ori.clear();
	song_name.clear();
	song_len.clear();
	int t;
	string tmp;
	//char tmp[1010];
	//setlocale(LC_ALL, "");
	ifstream fr("C:\\Users\\Administrator\\Desktop\\CQP\\data\\name.txt");
	ofstream fw("C:\\Users\\Administrator\\Desktop\\CQP\\data\\tst1.txt");
	for (int i = 0; i < fsize; i++)
	{
		getline(fr, tmp);
		//song_name.push_back(regex_replace(tmp, regex("\\s\\(\\d\\)"), ""));
		song_ori.push_back(tmp);
		song_name.push_back(regex_replace(tmp, regex("\\s\\(\\d\\)"), ""));
		fw << song_name[i] << "\n";
	}
	//fsize = tot-1;
	fr.close();
	fw.close();
	fr.open("C:\\Users\\Administrator\\Desktop\\CQP\\data\\length.txt");
	fw.open("C:\\Users\\Administrator\\Desktop\\CQP\\data\\tst2.txt");
	for (int i = 0; i < fsize; i++)
	{
		fr >> t;
		song_len.push_back(t);
		fw << song_name[i] << " " << song_len[i] << "\n";
	}
	fr.close();
	fw.close();
}

/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	game_status.clear();
	game_round.clear();
	song_num.clear();
	score_board.clear();
	QQ_st.clear();
	//files.clear();
	//getFiles(filePath, files);
	//fsize = files.size();
	//memset(song_name, 0, sizeof(song_name));
	//song_name.clear();
	//for (int i = 0; i < fsize; i++) song_name.push_back(SN(i));
	//memset(song_len, 0, sizeof(song_len));
	getFiles();
	game_cnt = 0;
	//ifstream fr("C:\\Users\\cjj490168650\\Desktop\\CQP\\data\\length.txt");
	//for (int i = 0; i < fsize; i++) fr >> song_len[i];
	//fr.close();
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
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
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
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
			//ss << song_len[i];
			//ss << SN(i).c_str() << "\n";
			//CQ_sendPrivateMsg(ac, fromQQ, SN(i).c_str());
			//CQ_sendPrivateMsg(ac, fromQQ, ss.str().c_str());
		}
		ss << song_name[fsize - 1] << " " << song_len[fsize - 1];
		s_tmp = ss.str();
		CQ_sendPrivateMsg(ac, fromQQ, s_tmp.c_str());
		//ss.str("");
		//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
		//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
		return EVENT_IGNORE;
	}
}


/*
* Type=2 Ⱥ��Ϣ
*/ 

int get_rand(int num)
{
	static mt19937 generator{ random_device()() };
	uniform_int_distribution<>distribution(0, num - 1);
	return distribution(generator);
}

int song_select(int len)
{
	//srand((int)time(0));
	int rand_song = get_rand(fsize);
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
	ss << "��ȷ�𰸣�" << song_name[song_num[fromGroup]];
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
	ss << "Round " << num << "   ��ֵ " << get_score(num);
	string s_tmp=ss.str();
	CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
	//ss.str("");
	s_tmp = "[CQ:record,file=" + song_name[song_num[fromGroup]] + ".mp3]";
	CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
	this_thread::sleep_for(chrono::duration<int>(20));
	if (game_round[fromGroup])
	{
		game_round[fromGroup] = 0;
		CQ_sendGroupMsg(ac, fromGroup, "��ʱ");
		send_ans(fromGroup); 
	}
}



CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	//if (!strcmp(msg,"[CQ:at,qq=3513312871] tst")) CQ_sendGroupMsg(ac,fromGroup,"[CQ:record,file=11-16 Loading ~Intro~.mp3]");
	if (fromQQ==490168650 && !strcmp(msg, "[CQ:at,qq=3513312871] ��������"))
	{
		//files.clear();
		//getFiles(filePath, files);
		//fsize = files.size();
		//memset(song_name, 0, sizeof(song_name));
		//song_name.clear();
		//for (int i = 0; i < fsize; i++) song_name[i] = SN(i);
		getFiles();
		CQ_sendGroupMsg(ac, fromGroup, "���³ɹ�");
		return EVENT_IGNORE;
	}
	if ((fromQQ == QQ_st[fromGroup] || fromQQ == 490168650) && !strcmp(msg, "[CQ:at,qq=3513312871] ��ֹ�¸�"))
	{
		game_status[fromGroup] = false;
		return EVENT_IGNORE;
	}
	if (!strcmp(msg, "[CQ:at,qq=3513312871] �¸�"))
	{
		if (game_status[fromGroup])
		{
			ostringstream ss;
			ss << "[CQ:at,qq=" << fromQQ << "] ������Ϸ��";
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
			AT(fromGroup, fromQQ, "��ǰȺ���ڽ���������Ϸ");
			ifs.close();
			return EVENT_IGNORE;
		}
		/*else if (game_cnt >= 3)
		{
			ostringstream ss;
			ss << "[CQ:at,qq=" << fromQQ << "] �Ѿ���3��Ⱥ������Ϸ�У����Ժ�����";
			string s_tmp = ss.str();
			CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
			//ss.str("");
		}*/
		game_status[fromGroup] = true;
		ofstream ofs(ss.str());
		ofs.close();
		game_cnt++;
		QQ_st[fromGroup] = fromQQ;
		CQ_sendGroupMsg(ac, fromGroup, "��������5���ַ���3�����֣�ƥ������ȷ��������5���ַ�����ȫ��ƥ�䡣");
		//CQ_sendGroupMsg(ac, fromGroup, "Ŀǰ��֧��ѡ����Ϸ");
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
		CQ_sendGroupMsg(ac, fromGroup, "��Ϸ����");
		ss.str("");
		map<int64_t, int>::iterator iter;
		ss<<"===�Ʒְ�===\n";
		for (iter = score_board[fromGroup].begin(); iter != score_board[fromGroup].end(); iter++)
		//for (int i = 0; i < score_board[fromGroup].size; i++)
		{
			ss << "[CQ:at,qq=" << iter->first << "] " << iter->second<<"\n";
		}
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
			ss << "[CQ:at,qq=" << fromQQ << "] �ش���ȷ��";
			string s_tmp = ss.str();
			CQ_sendGroupMsg(ac, fromGroup, s_tmp.c_str());
			//CQ_sendGroupMsg(ac, fromGroup,ss.str().c_str());
			//ss.str("");
			send_ans(fromGroup);
			//game_status[fromGroup]=false;
		}
		return EVENT_IGNORE;
	}
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "����menuA�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "����menuB�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}
