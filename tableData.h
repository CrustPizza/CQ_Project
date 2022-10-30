#pragma once
#include "parson.h"
#include <windows.h>
#include <atlstr.h>

enum class TableType
{
	ImageResource,
	Hero,
	Monster,
	Skill,
	Effect,
	Sound
};

enum class AnimType
{
	MOVE,
	ATTACK,
	DEATH,
	WIN,
	IDLE,
	Skill
};

struct GameRule
{
	int WaveMonsterIDs[15];
	int WaveMonsterNumber;
	int WaveDelay[15];
};

// ���ҽ� ���̺����� ���� (ID 10101001�� ĳ����, 20101001�� ����, 90001001�� ����Ʈ)
struct ImageResouceTable
{
	int Id;
	bool TransFormat;
	bool AlphaFormat;
	int AlphaValue;
	int Pivot[2];
	char ImageFileName[200];
};

// ĳ���� ���̺����� ���� (ID 101�� ���� ����)
struct HeroTable
{
	int Id;
	char Name[42];		// ĳ���� �̸� 20�� ����..(�ȳѰ��� ��)
	int HP;				// ���� ���� �߰��Ǹ� Array��..
	int ATK;
	char Skill_Icon[42];
	int Skill_ID[5];
	int HitBox[2];
	int AttackRange;
	// ĳ���� �ִϸ��̼� ���ҽ��� ������ [�̵�, ����, ����, �¸�, IDLE]����.  �߰��Ǹ� �迭 �ø��� ��.
	int AnimFirstIMG_ID[8];		// �� �ִϸ��̼��� ù��° BMP���ҽ�ID	
	int AnimFrame[8];			// �� �ִϸ��̼��� �� ������	
	int AnimDelay[8];			// �� �ִϸ��̼��� ������ (ũ�� ������)
};

struct SoundTable
{
	int Id;
	bool IsBgm;
	bool IsLoof;
	char SoundFileName[200];
};

// ���� ���̺����� ���� (ID 201�� ���� ����)
struct MonsterTable
{
	int Id;
	char Name[42];		// ĳ���� �̸� 20�� ����..(�ȳѰ��� ��)
	int HP;				// ���� ���� �߰��Ǹ� Array��..
	int ATK;
	int Skill_ID[5];
	int HitBox[2];
	int AttackRange;
	// ���� �ִϸ��̼� ���ҽ��� ������ [�̵�, ����, ����, �¸�, IDLE]����.  �߰��Ǹ� �迭 �ø��� ��.
	int AnimFirstIMG_ID[6];		// �� �ִϸ��̼��� ù��° BMP���ҽ�ID	
	int AnimFrame[6];			// �� �ִϸ��̼��� �� ������	
	int AnimDelay[6];			// �� �ִϸ��̼��� ������ (ũ�� ������)
};

// ��ų ���̺����� ���� (ID 310101�� ���� ����)
// ID ���� ���� - 3(��ų) + 101(����ĳ����ID) + 01 (���°��ų-���)
struct SkillTable
{
	int Id;
	char Name[42];		// ��ų �̸� 20�� ����..(�ȳѰ��� ��)
	char SkillType[42];		// ��ų Ÿ��
	float Rate;	// ��ų ����	
	int SkillEffectID;	// �ش� ��ų ����Ʈ �ִϸ��̼��� ID
};

// ����Ʈ ���̺����� ���� (ID 9001�� ���� ����)
// ID ���� ���� - 9(��ų) + 001(�����ѹ�)
struct EffectTable
{
	int Id;
	int EffectFirstIMG_ID;		// �ִϸ��̼��� ù��° BMP���ҽ�ID	
	bool isLoop;
	int EffectDuration;				// ������ �ִϸ��̼� ���� �ð�(������)	
	int EffectFrame;				// �ִϸ��̼��� �� ������	
	int EffectDelay;				// �ִϸ��̼��� ������ (ũ�� ������)
};

class DataTable
{
private:
	// �� Ŭ������ �ϳ��� ��ü�� �ν��Ͻ�ȭ & �̱������� ��� �ڵ������ �����ϵ���..
	static DataTable* SingletonObject;

	// �Ľ̵� ���̺� �����͵��� �� ��ü ����
	ImageResouceTable* imageResourceTable;	// �̹������ҽ�	
	HeroTable* heroTable;					// ĳ�������� (�ִϸ��̼� ���ҽ�ID����)
	MonsterTable* monsterTable;				// �������� (�ִϸ��̼� ���ҽ�ID����)
	SkillTable* skillTable;					// ��ų���� (���� ��ų����)
	EffectTable* effectTable;				// ����Ʈ ���ҽ� ���� (������ �׸���.. ���� �Ҹ���?)
	SoundTable* soundTable;					// ���� ���ҽ� ����
	GameRule gamerule;

public:
	// Ŭ���� ������ �Ҹ���
	DataTable();
	~DataTable();

	static DataTable* GetSingletonObject();
	static void		  ReleaseSingleton();

	// ���̺� �˻� �Լ���
	int FindIDfromIndex(int index, TableType tabletype);	// �ε������� ID���� ��ȯ
	int FindLastIndexOfTable(TableType tabletype);		// �����Ҵ�� �̹������ҽ��� ������ ��ȯ
	const ImageResouceTable* FindImageDataFromID(int imageID);	
	const HeroTable* FindHeroDataFromID(int characterID);	
	const MonsterTable* FindMonsterDataFromID(int monsterID);	
	const SkillTable* FindSkillDataFromID(int skillID);	
	const EffectTable* FindEffectDataFromID(int effectID);
	const SoundTable* FindSoundDataFromID(int soundID);
	const GameRule* FindGameRuleData();


private:
	// json ���� �Ľ� �Լ�
	ImageResouceTable* JsonParse_ImageResourceData();
	HeroTable* JsonParse_HeroData();
	MonsterTable* JsonParse_MonsterData();
	SkillTable* JsonParse_SkillData();
	EffectTable* JsonParse_EffectData();
	SoundTable* JsonParse_SoundData();
	void JsonParse_GameRule();

	// �޸� ���� �Լ�
	void Release();

};