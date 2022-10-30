#include "stdafx.h"
#include "tableData.h"

string path = "..//..//..//4_Resources//contents//";

DataTable* DataTable::SingletonObject = nullptr;

// 초기화 생성자, 소멸자
DataTable::DataTable()
{
	imageResourceTable = JsonParse_ImageResourceData();
	heroTable = JsonParse_HeroData();
	monsterTable = JsonParse_MonsterData();
	skillTable = JsonParse_SkillData();
	effectTable = JsonParse_EffectData();
	soundTable = JsonParse_SoundData();
	JsonParse_GameRule();

}
DataTable::~DataTable()
{
	Release();
}

DataTable* DataTable::GetSingletonObject()
{
	if (SingletonObject == nullptr)
	{
		SingletonObject = new DataTable();
	}
	return SingletonObject;
}

void DataTable::ReleaseSingleton()
{
	if (SingletonObject != nullptr)
	{
		SingletonObject->Release();
		delete SingletonObject;
		SingletonObject = nullptr;
	}
}

void DataTable::JsonParse_GameRule()
{
	JSON_Value* rootValue;
	JSON_Object* rootObject;
	string fileName = "GameRule.json";

	JSON_Array* WaveMonsterIDs;
	JSON_Array* WaveDelay;	

	rootValue = json_parse_file((path + fileName).c_str());
	rootObject = json_value_get_object(rootValue);
		
	// 어레이 형태 데이터 오브젝트 파싱
	WaveMonsterIDs = json_object_get_array(rootObject, "WaveMonsterIDs");	
	WaveDelay = json_object_get_array(rootObject, "WaveDelay");

	gamerule.WaveMonsterNumber = (int)json_array_get_count(WaveMonsterIDs);

	// 각각 데이터들을 구조체로..
	for (int y = 0; y < gamerule.WaveMonsterNumber; y++)
	{
		gamerule.WaveMonsterIDs[y] = (int)json_array_get_number(WaveMonsterIDs, y);
	}
	for (int y = 0; y < (int)json_array_get_count(WaveDelay); y++)
	{
		gamerule.WaveDelay[y] = (int)json_array_get_number(WaveDelay, y);
	}
	json_value_free(rootValue);
}

// 외부파일(ImageResourceTable)을 불러와 동적배열에 데이터를 파싱하여 넣은 후, 객체 포인터 리턴
ImageResouceTable* DataTable::JsonParse_ImageResourceData()
{
	JSON_Value* rootValue;
	JSON_Array* rootObjects;
	JSON_Object* rootObject;

	JSON_Array* Pivot;
	string fileName = "ImageResourceTable.json";

	rootValue = json_parse_file((path + fileName).c_str());
	rootObjects = json_value_get_array(rootValue); // array가 가장 먼저 시작되니 value에서 array를 받음

	ImageResouceTable* imageResourceData
		= new ImageResouceTable[(int)json_array_get_count(rootObjects)];	// 객체 동적 생성		

// 어레이 파싱된 데이터에서 각 인덱스 순서로 객체의 배열에 값을 넣어줌.
	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array의 인덱스별로, rootobject로 파싱
		rootObject = json_array_get_object(rootObjects, i);

		// 배열 아닌 데이터들은 태그로 질의하여 값을 받아 객체에 저장
		imageResourceData[i].Id = (int)json_object_get_number(rootObject, "Id");
		imageResourceData[i].TransFormat = (bool)json_object_get_boolean(rootObject, "TransFormat");
		imageResourceData[i].AlphaFormat = (bool)json_object_get_boolean(rootObject, "AlphaFormat");
		imageResourceData[i].AlphaValue = (int)json_object_get_number(rootObject, "AlphaValue");
		strcpy_s(imageResourceData[i].ImageFileName, (char*)json_object_get_string(rootObject, "ImageFileName"));

		// 배열인 데이터는 array형태로 객체를 받고, array개수 만큼 반복하여 데이터를 객체에 저장
		Pivot = json_object_get_array(rootObject, "Pivot");
		for (int y = 0; y < (int)json_array_get_count(Pivot); y++)
		{
			imageResourceData[i].Pivot[y] = (int)json_array_get_number(Pivot, y);
		}
	}
	// 파싱하느라 사용했던 루트 해제
	json_value_free(rootValue);
	// 동적 생성된 데이터의 주소를 반환
	return imageResourceData;
}

// 외부파일(HeroTable)을 불러와 동적배열에 데이터를 파싱하여 넣은 후, 객체 포인터 리턴
HeroTable* DataTable::JsonParse_HeroData()
{
	JSON_Value* rootValue;
	JSON_Array* rootObjects;
	JSON_Object* rootObject;

	JSON_Array* animfirstimgIDs;
	JSON_Array* animframes;
	JSON_Array* animdelays;
	JSON_Array* skill_IDs;
	JSON_Array* hitbox;

	string fileName = "HeroTable.json";

	rootValue = json_parse_file((path + fileName).c_str());
	rootObjects = json_value_get_array(rootValue);

	HeroTable* characterData
		= new HeroTable[(int)json_array_get_count(rootObjects)];	// 객체 동적 생성	

	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array의 인덱스별로, rootobject로 파싱
		rootObject = json_array_get_object(rootObjects, i);

		// 배열 아닌 데이터들은 태그로 질의하여 값을 받아 객체에 저장
		characterData[i].Id = (int)json_object_get_number(rootObject, "Id");
		strcpy_s(characterData[i].Name, (char*)json_object_get_string(rootObject, "Name"));
		characterData[i].HP = (int)json_object_get_number(rootObject, "HP");
		characterData[i].AttackRange = (int)json_object_get_number(rootObject, "AttackRange");
		characterData[i].ATK = (int)json_object_get_number(rootObject, "ATK");
		strcpy_s(characterData[i].Skill_Icon, (char*)json_object_get_string(rootObject, "Skill_Icon"));		

		// 배열인 데이터는 array형태로 객체를 받고, array개수 만큼 반복하여 데이터를 객체에 저장
		animfirstimgIDs = json_object_get_array(rootObject, "AnimFirstIMG_ID");
		animframes = json_object_get_array(rootObject, "AnimFrame");
		animdelays = json_object_get_array(rootObject, "AnimDelay");
		skill_IDs = json_object_get_array(rootObject, "Skill_ID");
		hitbox = json_object_get_array(rootObject, "HitBox");

		// 이하 데이터들은 캐릭터들의 애니메이션에 대한 것. 애니메이션 갯수가 같으므로 한번에 처리
		for (int y = 0; y < (int)json_array_get_count(animfirstimgIDs); y++)
		{
			characterData[i].AnimFirstIMG_ID[y] = (int)json_array_get_number(animfirstimgIDs, y);
			characterData[i].AnimFrame[y] = (int)json_array_get_number(animframes, y);
			characterData[i].AnimDelay[y] = (int)json_array_get_number(animdelays, y);
		}
		// 스킬 ID들 입력
		for (int y = 0; y < (int)json_array_get_count(skill_IDs); y++)
		{
			characterData[i].Skill_ID[y] = (int)json_array_get_number(skill_IDs, y);
		}
		// 스킬 ID들 입력
		for (int y = 0; y < (int)json_array_get_count(hitbox); y++)
		{
			characterData[i].HitBox[y] = (int)json_array_get_number(hitbox, y);
		}
	}
	json_value_free(rootValue);
	return characterData;
}

// 외부파일(MonsterTable)을 불러와 동적배열에 데이터를 파싱하여 넣은 후, 객체 포인터 리턴
MonsterTable* DataTable::JsonParse_MonsterData()
{
	JSON_Value* rootValue;
	JSON_Array* rootObjects;
	JSON_Object* rootObject;

	JSON_Array* skill_ID;
	JSON_Array* hitbox;
	JSON_Array* animfirstimgIDs;
	JSON_Array* animframes;
	JSON_Array* animdelays;

	string fileName = "MonsterTable.json";

	rootValue = json_parse_file((path + fileName).c_str());
	rootObjects = json_value_get_array(rootValue);

	MonsterTable* monsterData
		= new MonsterTable[(int)json_array_get_count(rootObjects)];	// 객체 동적 생성	

	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array의 인덱스별로, rootobject로 파싱
		rootObject = json_array_get_object(rootObjects, i);

		// 배열 아닌 데이터들은 태그로 질의하여 값을 받아 객체에 저장
		monsterData[i].Id = (int)json_object_get_number(rootObject, "Id");
		strcpy_s(monsterData[i].Name, (char*)json_object_get_string(rootObject, "Name"));
		monsterData[i].HP = (int)json_object_get_number(rootObject, "HP");
		monsterData[i].ATK = (int)json_object_get_number(rootObject, "ATK");
		monsterData[i].AttackRange = (int)json_object_get_number(rootObject, "AttackRange");
		
		// 배열인 데이터는 array형태로 객체를 받고, array개수 만큼 반복하여 데이터를 객체에 저장
		animfirstimgIDs = json_object_get_array(rootObject, "AnimFirstIMG_ID");
		animframes = json_object_get_array(rootObject, "AnimFrame");
		animdelays = json_object_get_array(rootObject, "AnimDelay");
		skill_ID = json_object_get_array(rootObject, "Skill_ID");
		hitbox = json_object_get_array(rootObject, "HitBox");

		// 이하 데이터들은 캐릭터들의 애니메이션에 대한 것. 애니메이션 갯수가 같으므로 한번에 처리
		for (int y = 0; y < (int)json_array_get_count(animfirstimgIDs); y++)
		{
			monsterData[i].AnimFirstIMG_ID[y] = (int)json_array_get_number(animfirstimgIDs, y);
			monsterData[i].AnimFrame[y] = (int)json_array_get_number(animframes, y);
			monsterData[i].AnimDelay[y] = (int)json_array_get_number(animdelays, y);
		}
		// 그 외 어레이 데이터
		for (int y = 0; y < (int)json_array_get_count(skill_ID); y++)
		{
			monsterData[i].Skill_ID[y] = (int)json_array_get_number(skill_ID, y);
		}		
		for (int y = 0; y < (int)json_array_get_count(hitbox); y++)
		{
			monsterData[i].HitBox[y] = (int)json_array_get_number(hitbox, y);
		}

	}
	json_value_free(rootValue);
	return monsterData;
}

// 외부파일(SkillTable)을 불러와 동적배열에 데이터를 파싱하여 넣은 후, 객체 포인터 리턴
SkillTable* DataTable::JsonParse_SkillData()
{
	JSON_Value* rootValue;
	JSON_Array* rootObjects;
	JSON_Object* rootObject;

	string fileName = "SkillTable.json";

	rootValue = json_parse_file((path + fileName).c_str());
	rootObjects = json_value_get_array(rootValue);

	SkillTable* skillData
		= new SkillTable[(int)json_array_get_count(rootObjects)];	// 객체 동적 생성	

	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array의 인덱스별로, rootobject로 파싱
		rootObject = json_array_get_object(rootObjects, i);

		// 배열 아닌 데이터들은 태그로 질의하여 값을 받아 객체에 저장
		skillData[i].Id = (int)json_object_get_number(rootObject, "Id");
		strcpy_s(skillData[i].Name, (char*)json_object_get_string(rootObject, "Name"));
		strcpy_s(skillData[i].SkillType, (char*)json_object_get_string(rootObject, "SkillType"));
		skillData[i].Rate = (float)json_object_get_number(rootObject, "Rate");
		skillData[i].SkillEffectID = (int)json_object_get_number(rootObject, "SkillEffectID");
	}
	json_value_free(rootValue);
	return skillData;
}

// 외부파일(EffectTable)을 불러와 동적배열에 데이터를 파싱하여 넣은 후, 객체 포인터 리턴
EffectTable* DataTable::JsonParse_EffectData()
{
	JSON_Value* rootValue;
	JSON_Array* rootObjects;
	JSON_Object* rootObject;

	string fileName = "EffectTable.json";

	rootValue = json_parse_file((path + fileName).c_str());
	rootObjects = json_value_get_array(rootValue);

	EffectTable* effectData
		= new EffectTable[(int)json_array_get_count(rootObjects)];	// 객체 동적 생성	

	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array의 인덱스별로, rootobject로 파싱
		rootObject = json_array_get_object(rootObjects, i);

		// 배열 아닌 데이터들은 태그로 질의하여 값을 받아 객체에 저장
		effectData[i].Id = (int)json_object_get_number(rootObject, "Id");
		effectData[i].EffectFirstIMG_ID = (int)json_object_get_number(rootObject, "EffectFirstIMG_ID");
		effectData[i].isLoop = (bool)json_object_get_boolean(rootObject, "IsLoop");
		effectData[i].EffectDuration = (int)json_object_get_number(rootObject, "EffectDuration");
		effectData[i].EffectFrame = (int)json_object_get_number(rootObject, "EffectFrame");
		effectData[i].EffectDelay = (int)json_object_get_number(rootObject, "EffectDelay");
	}
	json_value_free(rootValue);
	return effectData;
}

SoundTable* DataTable::JsonParse_SoundData()
{
	JSON_Value* rootValue;
	JSON_Array* rootObjects;
	JSON_Object* rootObject;

	string fileName = "SoundTable.json";

	rootValue = json_parse_file((path + fileName).c_str());
	rootObjects = json_value_get_array(rootValue);

	SoundTable* soundData
		= new SoundTable[(int)json_array_get_count(rootObjects)];	// 객체 동적 생성	

	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array의 인덱스별로, rootobject로 파싱
		rootObject = json_array_get_object(rootObjects, i);

		// 배열 아닌 데이터들은 태그로 질의하여 값을 받아 객체에 저장
		soundData[i].Id = (int)json_object_get_number(rootObject, "Id");
		soundData[i].IsLoof = (int)json_object_get_boolean(rootObject, "IsLoof");
		soundData[i].IsBgm = (int)json_object_get_boolean(rootObject, "IsBgm");
		strcpy_s(soundData[i].SoundFileName, (char*)json_object_get_string(rootObject, "SoundFileName"));		 
	}
	json_value_free(rootValue);
	return soundData;
}

const ImageResouceTable* DataTable::FindImageDataFromID(int imageID)
{
	for (int i = 0;
		(i < _msize(imageResourceTable) / sizeof(imageResourceTable[0]));
		i++)
	{
		if ((int)imageResourceTable[i].Id == (int)imageID)
		{
			return &imageResourceTable[i];
		}
	}
	return nullptr;
}

int DataTable::FindIDfromIndex(int index, TableType tabletype)
{
	if (tabletype == TableType::ImageResource)
	{
		for (int i = 0; (i < _msize(imageResourceTable) / sizeof(imageResourceTable[0])); i++)
		{
			if (i == index)
			{
				return imageResourceTable[i].Id;
			}
		}
	}
	else if (tabletype == TableType::Hero)
	{
		for (int i = 0; (i < _msize(heroTable) / sizeof(heroTable[0])); i++)
		{
			if (i == index)
			{
				return heroTable[i].Id;
			}
		}
	}
	else if (tabletype == TableType::Monster)
	{
		for (int i = 0; (i < _msize(monsterTable) / sizeof(monsterTable[0])); i++)
		{
			if (i == index)
			{
				return monsterTable[i].Id;
			}
		}
	}

	else if (tabletype == TableType::Sound)
	{
		for (int i = 0; (i < _msize(soundTable) / sizeof(soundTable[0])); i++)
		{
			if (i == index)
			{
				return soundTable[i].Id;
			}
		}
	}

	return 0;
}

int DataTable::FindLastIndexOfTable(TableType tabletype)
{
	if (tabletype == TableType::ImageResource)
		return _msize(imageResourceTable) / sizeof(imageResourceTable[0]);
	else if (tabletype == TableType::Hero)
		return _msize(heroTable) / sizeof(heroTable[0]);
	else if (tabletype == TableType::Monster)
		return _msize(monsterTable) / sizeof(monsterTable[0]);
	else if (tabletype == TableType::Skill)
		return _msize(skillTable) / sizeof(skillTable[0]);
	else if (tabletype == TableType::Effect)
		return _msize(effectTable) / sizeof(effectTable[0]);
	else if (tabletype == TableType::Sound)
		return _msize(soundTable) / sizeof(soundTable[0]);
	else
		return 0;	
}

const GameRule* DataTable::FindGameRuleData()
{
	return &gamerule;
}

const SoundTable* DataTable::FindSoundDataFromID(int soundID)
{
	for (int i = 0;
		(i < _msize(soundTable) / sizeof(soundTable[0]));
		i++)
	{
		if ((int)soundTable[i].Id == (int)soundID)
		{
			return &soundTable[i];
		}
	}
	return nullptr;
}

const HeroTable* DataTable::FindHeroDataFromID(int characterID)
{
	for (int i = 0;
		(i < _msize(heroTable) / sizeof(heroTable[0]));
		i++)
	{
		if ((int)heroTable[i].Id == (int)characterID)
		{
			return &heroTable[i];
		}
	}
	return nullptr;
}

const MonsterTable* DataTable::FindMonsterDataFromID(int monsterID)
{
	for (int i = 0;
		(i < _msize(monsterTable) / sizeof(monsterTable[0]));
		i++)
	{
		if ((int)monsterTable[i].Id == (int)monsterID)
		{
			return &monsterTable[i];
		}
	}
	return nullptr;
}

const SkillTable* DataTable::FindSkillDataFromID(int skillID)
{
	for (int i = 0;
		(i < _msize(skillTable) / sizeof(skillTable[0]));
		i++)
	{
		if ((int)skillTable[i].Id == (int)skillID)
		{
			return &skillTable[i];
		}
	}

	return nullptr;
}

const EffectTable* DataTable::FindEffectDataFromID(int effectID)
{
	for (int i = 0;
		(i < _msize(effectTable) / sizeof(effectTable[0]));
		i++)
	{
		if ((int)effectTable[i].Id == (int)effectID)
		{
			return &effectTable[i];
		}
	}
	return nullptr;
}

void DataTable::Release()
{
	delete(imageResourceTable);
	imageResourceTable = nullptr;
	delete(heroTable);
	heroTable = nullptr;
	delete(monsterTable);
	monsterTable = nullptr;
	delete(skillTable);
	skillTable = nullptr;
	delete(effectTable);
	effectTable = nullptr;
	delete(soundTable);
	soundTable = nullptr;
}