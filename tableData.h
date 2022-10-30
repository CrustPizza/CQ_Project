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

// 리소스 테이블데이터 구조 (ID 10101001대 캐릭터, 20101001대 몬스터, 90001001대 이펙트)
struct ImageResouceTable
{
	int Id;
	bool TransFormat;
	bool AlphaFormat;
	int AlphaValue;
	int Pivot[2];
	char ImageFileName[200];
};

// 캐릭터 테이블데이터 구조 (ID 101번 부터 시작)
struct HeroTable
{
	int Id;
	char Name[42];		// 캐릭터 이름 20자 까지..(안넘겠지 뭐)
	int HP;				// 추후 레벨 추가되면 Array로..
	int ATK;
	char Skill_Icon[42];
	int Skill_ID[5];
	int HitBox[2];
	int AttackRange;
	// 캐릭터 애니메이션 리소스의 데이터 [이동, 공격, 죽음, 승리, IDLE]순서.  추가되면 배열 늘리면 됨.
	int AnimFirstIMG_ID[8];		// 각 애니메이션의 첫번째 BMP리소스ID	
	int AnimFrame[8];			// 각 애니메이션의 총 프레임	
	int AnimDelay[8];			// 각 애니메이션의 딜레이 (크면 느려짐)
};

struct SoundTable
{
	int Id;
	bool IsBgm;
	bool IsLoof;
	char SoundFileName[200];
};

// 몬스터 테이블데이터 구조 (ID 201번 부터 시작)
struct MonsterTable
{
	int Id;
	char Name[42];		// 캐릭터 이름 20자 까지..(안넘겠지 뭐)
	int HP;				// 추후 레벨 추가되면 Array로..
	int ATK;
	int Skill_ID[5];
	int HitBox[2];
	int AttackRange;
	// 몬스터 애니메이션 리소스의 데이터 [이동, 공격, 죽음, 승리, IDLE]순서.  추가되면 배열 늘리면 됨.
	int AnimFirstIMG_ID[6];		// 각 애니메이션의 첫번째 BMP리소스ID	
	int AnimFrame[6];			// 각 애니메이션의 총 프레임	
	int AnimDelay[6];			// 각 애니메이션의 딜레이 (크면 느려짐)
};

// 스킬 테이블데이터 구조 (ID 310101번 부터 시작)
// ID 구성 이해 - 3(스킬) + 101(보유캐릭터ID) + 01 (몇번째스킬-등급)
struct SkillTable
{
	int Id;
	char Name[42];		// 스킬 이름 20자 까지..(안넘겠지 뭐)
	char SkillType[42];		// 스킬 타입
	float Rate;	// 스킬 배율	
	int SkillEffectID;	// 해당 스킬 이펙트 애니메이션의 ID
};

// 이펙트 테이블데이터 구조 (ID 9001번 부터 시작)
// ID 구성 이해 - 9(스킬) + 001(매직넘버)
struct EffectTable
{
	int Id;
	int EffectFirstIMG_ID;		// 애니메이션의 첫번째 BMP리소스ID	
	bool isLoop;
	int EffectDuration;				// 루프형 애니메이션 지속 시간(프레임)	
	int EffectFrame;				// 애니메이션의 총 프레임	
	int EffectDelay;				// 애니메이션의 딜레이 (크면 느려짐)
};

class DataTable
{
private:
	// 본 클래스도 하나의 객체만 인스턴스화 & 싱글콘으로 모든 코드블럭에서 접근하도록..
	static DataTable* SingletonObject;

	// 파싱된 테이블 데이터들이 들어갈 객체 선언
	ImageResouceTable* imageResourceTable;	// 이미지리소스	
	HeroTable* heroTable;					// 캐릭터정보 (애니메이션 리소스ID포함)
	MonsterTable* monsterTable;				// 몬스터정보 (애니메이션 리소스ID포함)
	SkillTable* skillTable;					// 스킬정보 (몬스터 스킬포함)
	EffectTable* effectTable;				// 이펙트 리소스 정보 (지금은 그림만.. 추후 소리도?)
	SoundTable* soundTable;					// 사운드 리소스 정보
	GameRule gamerule;

public:
	// 클래스 생성자 소멸자
	DataTable();
	~DataTable();

	static DataTable* GetSingletonObject();
	static void		  ReleaseSingleton();

	// 테이블 검색 함수들
	int FindIDfromIndex(int index, TableType tabletype);	// 인덱스에서 ID값을 반환
	int FindLastIndexOfTable(TableType tabletype);		// 동적할당된 이미지리소스의 개수를 반환
	const ImageResouceTable* FindImageDataFromID(int imageID);	
	const HeroTable* FindHeroDataFromID(int characterID);	
	const MonsterTable* FindMonsterDataFromID(int monsterID);	
	const SkillTable* FindSkillDataFromID(int skillID);	
	const EffectTable* FindEffectDataFromID(int effectID);
	const SoundTable* FindSoundDataFromID(int soundID);
	const GameRule* FindGameRuleData();


private:
	// json 파일 파싱 함수
	ImageResouceTable* JsonParse_ImageResourceData();
	HeroTable* JsonParse_HeroData();
	MonsterTable* JsonParse_MonsterData();
	SkillTable* JsonParse_SkillData();
	EffectTable* JsonParse_EffectData();
	SoundTable* JsonParse_SoundData();
	void JsonParse_GameRule();

	// 메모리 해제 함수
	void Release();

};