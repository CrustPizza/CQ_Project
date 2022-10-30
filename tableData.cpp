#include "stdafx.h"
#include "tableData.h"

string path = "..//..//..//4_Resources//contents//";

DataTable* DataTable::SingletonObject = nullptr;

// �ʱ�ȭ ������, �Ҹ���
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
		
	// ��� ���� ������ ������Ʈ �Ľ�
	WaveMonsterIDs = json_object_get_array(rootObject, "WaveMonsterIDs");	
	WaveDelay = json_object_get_array(rootObject, "WaveDelay");

	gamerule.WaveMonsterNumber = (int)json_array_get_count(WaveMonsterIDs);

	// ���� �����͵��� ����ü��..
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

// �ܺ�����(ImageResourceTable)�� �ҷ��� �����迭�� �����͸� �Ľ��Ͽ� ���� ��, ��ü ������ ����
ImageResouceTable* DataTable::JsonParse_ImageResourceData()
{
	JSON_Value* rootValue;
	JSON_Array* rootObjects;
	JSON_Object* rootObject;

	JSON_Array* Pivot;
	string fileName = "ImageResourceTable.json";

	rootValue = json_parse_file((path + fileName).c_str());
	rootObjects = json_value_get_array(rootValue); // array�� ���� ���� ���۵Ǵ� value���� array�� ����

	ImageResouceTable* imageResourceData
		= new ImageResouceTable[(int)json_array_get_count(rootObjects)];	// ��ü ���� ����		

// ��� �Ľ̵� �����Ϳ��� �� �ε��� ������ ��ü�� �迭�� ���� �־���.
	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array�� �ε�������, rootobject�� �Ľ�
		rootObject = json_array_get_object(rootObjects, i);

		// �迭 �ƴ� �����͵��� �±׷� �����Ͽ� ���� �޾� ��ü�� ����
		imageResourceData[i].Id = (int)json_object_get_number(rootObject, "Id");
		imageResourceData[i].TransFormat = (bool)json_object_get_boolean(rootObject, "TransFormat");
		imageResourceData[i].AlphaFormat = (bool)json_object_get_boolean(rootObject, "AlphaFormat");
		imageResourceData[i].AlphaValue = (int)json_object_get_number(rootObject, "AlphaValue");
		strcpy_s(imageResourceData[i].ImageFileName, (char*)json_object_get_string(rootObject, "ImageFileName"));

		// �迭�� �����ʹ� array���·� ��ü�� �ް�, array���� ��ŭ �ݺ��Ͽ� �����͸� ��ü�� ����
		Pivot = json_object_get_array(rootObject, "Pivot");
		for (int y = 0; y < (int)json_array_get_count(Pivot); y++)
		{
			imageResourceData[i].Pivot[y] = (int)json_array_get_number(Pivot, y);
		}
	}
	// �Ľ��ϴ��� ����ߴ� ��Ʈ ����
	json_value_free(rootValue);
	// ���� ������ �������� �ּҸ� ��ȯ
	return imageResourceData;
}

// �ܺ�����(HeroTable)�� �ҷ��� �����迭�� �����͸� �Ľ��Ͽ� ���� ��, ��ü ������ ����
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
		= new HeroTable[(int)json_array_get_count(rootObjects)];	// ��ü ���� ����	

	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array�� �ε�������, rootobject�� �Ľ�
		rootObject = json_array_get_object(rootObjects, i);

		// �迭 �ƴ� �����͵��� �±׷� �����Ͽ� ���� �޾� ��ü�� ����
		characterData[i].Id = (int)json_object_get_number(rootObject, "Id");
		strcpy_s(characterData[i].Name, (char*)json_object_get_string(rootObject, "Name"));
		characterData[i].HP = (int)json_object_get_number(rootObject, "HP");
		characterData[i].AttackRange = (int)json_object_get_number(rootObject, "AttackRange");
		characterData[i].ATK = (int)json_object_get_number(rootObject, "ATK");
		strcpy_s(characterData[i].Skill_Icon, (char*)json_object_get_string(rootObject, "Skill_Icon"));		

		// �迭�� �����ʹ� array���·� ��ü�� �ް�, array���� ��ŭ �ݺ��Ͽ� �����͸� ��ü�� ����
		animfirstimgIDs = json_object_get_array(rootObject, "AnimFirstIMG_ID");
		animframes = json_object_get_array(rootObject, "AnimFrame");
		animdelays = json_object_get_array(rootObject, "AnimDelay");
		skill_IDs = json_object_get_array(rootObject, "Skill_ID");
		hitbox = json_object_get_array(rootObject, "HitBox");

		// ���� �����͵��� ĳ���͵��� �ִϸ��̼ǿ� ���� ��. �ִϸ��̼� ������ �����Ƿ� �ѹ��� ó��
		for (int y = 0; y < (int)json_array_get_count(animfirstimgIDs); y++)
		{
			characterData[i].AnimFirstIMG_ID[y] = (int)json_array_get_number(animfirstimgIDs, y);
			characterData[i].AnimFrame[y] = (int)json_array_get_number(animframes, y);
			characterData[i].AnimDelay[y] = (int)json_array_get_number(animdelays, y);
		}
		// ��ų ID�� �Է�
		for (int y = 0; y < (int)json_array_get_count(skill_IDs); y++)
		{
			characterData[i].Skill_ID[y] = (int)json_array_get_number(skill_IDs, y);
		}
		// ��ų ID�� �Է�
		for (int y = 0; y < (int)json_array_get_count(hitbox); y++)
		{
			characterData[i].HitBox[y] = (int)json_array_get_number(hitbox, y);
		}
	}
	json_value_free(rootValue);
	return characterData;
}

// �ܺ�����(MonsterTable)�� �ҷ��� �����迭�� �����͸� �Ľ��Ͽ� ���� ��, ��ü ������ ����
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
		= new MonsterTable[(int)json_array_get_count(rootObjects)];	// ��ü ���� ����	

	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array�� �ε�������, rootobject�� �Ľ�
		rootObject = json_array_get_object(rootObjects, i);

		// �迭 �ƴ� �����͵��� �±׷� �����Ͽ� ���� �޾� ��ü�� ����
		monsterData[i].Id = (int)json_object_get_number(rootObject, "Id");
		strcpy_s(monsterData[i].Name, (char*)json_object_get_string(rootObject, "Name"));
		monsterData[i].HP = (int)json_object_get_number(rootObject, "HP");
		monsterData[i].ATK = (int)json_object_get_number(rootObject, "ATK");
		monsterData[i].AttackRange = (int)json_object_get_number(rootObject, "AttackRange");
		
		// �迭�� �����ʹ� array���·� ��ü�� �ް�, array���� ��ŭ �ݺ��Ͽ� �����͸� ��ü�� ����
		animfirstimgIDs = json_object_get_array(rootObject, "AnimFirstIMG_ID");
		animframes = json_object_get_array(rootObject, "AnimFrame");
		animdelays = json_object_get_array(rootObject, "AnimDelay");
		skill_ID = json_object_get_array(rootObject, "Skill_ID");
		hitbox = json_object_get_array(rootObject, "HitBox");

		// ���� �����͵��� ĳ���͵��� �ִϸ��̼ǿ� ���� ��. �ִϸ��̼� ������ �����Ƿ� �ѹ��� ó��
		for (int y = 0; y < (int)json_array_get_count(animfirstimgIDs); y++)
		{
			monsterData[i].AnimFirstIMG_ID[y] = (int)json_array_get_number(animfirstimgIDs, y);
			monsterData[i].AnimFrame[y] = (int)json_array_get_number(animframes, y);
			monsterData[i].AnimDelay[y] = (int)json_array_get_number(animdelays, y);
		}
		// �� �� ��� ������
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

// �ܺ�����(SkillTable)�� �ҷ��� �����迭�� �����͸� �Ľ��Ͽ� ���� ��, ��ü ������ ����
SkillTable* DataTable::JsonParse_SkillData()
{
	JSON_Value* rootValue;
	JSON_Array* rootObjects;
	JSON_Object* rootObject;

	string fileName = "SkillTable.json";

	rootValue = json_parse_file((path + fileName).c_str());
	rootObjects = json_value_get_array(rootValue);

	SkillTable* skillData
		= new SkillTable[(int)json_array_get_count(rootObjects)];	// ��ü ���� ����	

	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array�� �ε�������, rootobject�� �Ľ�
		rootObject = json_array_get_object(rootObjects, i);

		// �迭 �ƴ� �����͵��� �±׷� �����Ͽ� ���� �޾� ��ü�� ����
		skillData[i].Id = (int)json_object_get_number(rootObject, "Id");
		strcpy_s(skillData[i].Name, (char*)json_object_get_string(rootObject, "Name"));
		strcpy_s(skillData[i].SkillType, (char*)json_object_get_string(rootObject, "SkillType"));
		skillData[i].Rate = (float)json_object_get_number(rootObject, "Rate");
		skillData[i].SkillEffectID = (int)json_object_get_number(rootObject, "SkillEffectID");
	}
	json_value_free(rootValue);
	return skillData;
}

// �ܺ�����(EffectTable)�� �ҷ��� �����迭�� �����͸� �Ľ��Ͽ� ���� ��, ��ü ������ ����
EffectTable* DataTable::JsonParse_EffectData()
{
	JSON_Value* rootValue;
	JSON_Array* rootObjects;
	JSON_Object* rootObject;

	string fileName = "EffectTable.json";

	rootValue = json_parse_file((path + fileName).c_str());
	rootObjects = json_value_get_array(rootValue);

	EffectTable* effectData
		= new EffectTable[(int)json_array_get_count(rootObjects)];	// ��ü ���� ����	

	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array�� �ε�������, rootobject�� �Ľ�
		rootObject = json_array_get_object(rootObjects, i);

		// �迭 �ƴ� �����͵��� �±׷� �����Ͽ� ���� �޾� ��ü�� ����
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
		= new SoundTable[(int)json_array_get_count(rootObjects)];	// ��ü ���� ����	

	for (int i = 0; i < (int)json_array_get_count(rootObjects); i++)
	{
		// json array�� �ε�������, rootobject�� �Ľ�
		rootObject = json_array_get_object(rootObjects, i);

		// �迭 �ƴ� �����͵��� �±׷� �����Ͽ� ���� �޾� ��ü�� ����
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