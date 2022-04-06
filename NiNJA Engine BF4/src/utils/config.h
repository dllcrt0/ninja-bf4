#pragma once

class IniParse {
public:
	IniParse() {}

	IniParse(const char* pFileName) {
		DbgPrint("Attempting to parse %s as ini", pFileName);

		auto file = fopen(pFileName, "r");
		if (file) {
			bIsGood = true;
			DbgPrint("%s is a valid file, and is being read", pFileName);
			string currentSection;
			int currentLine = 0;
			char currentDilem = '=';

			char line[300];
			while (fgets(line, 300, file) != 0) {
				string strLine(line);
				strLine.erase(std::remove(strLine.begin(), strLine.end(), '\n'), strLine.end());
				if (strLine.empty()) continue;

				DbgPrint("Read line: %s", strLine.c_str());

				currentLine++;
				currentDilem = '=';
				bool checkedDilem = false;

				if (line[0] == '[') {
					// section line
					currentSection = strLine.substr(1, strLine.length() - 2);
				} else {
					if (line[0] == NULL) {
						continue;
					}

					if (currentSection.c_str()) {
						bool isSpacedLeft = false;
						bool isSpacedRight = false;

					jStart:
						int equals = strLine.find(currentDilem);
						if (equals > 0) {
							if (line[equals - 1] == ' ') {
								isSpacedLeft = true;
							}

							if (line[equals + 1] == ' ') {
								isSpacedRight = true;
							}

							string key = strLine.substr(0, equals - (isSpacedLeft ? 1 : 0));
							string value = strLine.substr(equals + (isSpacedRight ? 2 : 1));

							if (value.length() <= 0) {
								DbgPrint("IniParse::Warning - %s::%s has no value", currentSection.c_str(), key.c_str());
								continue;
							}

							ItemMap.insert(make_pair(make_pair(currentSection, key), value));
						} else {
							if (strLine.find(':') <= 0) {
								DbgPrint("IniParse::Error - oops, seems like you forgot the equals sign on an item in %s on line %i", currentSection, currentLine);
							} else {
								if (checkedDilem) continue;
								checkedDilem = true;

								currentDilem = ':';
								goto jStart;
							}
						}
					} else {
						DbgPrint("IniParse::Error - oops, coundn't find a valid first section");
					}
				}
			}

			fclose(file);
		} else {
			DbgPrint("File couldn't be opened..");
		}
	}

	vector<string> GetKeys() {
		vector<string> mapped;

		for (auto outer = ItemMap.begin(); outer != ItemMap.end(); ++outer) {
			mapped.push_back(outer->first.second);
		}

		return mapped;
	}

	vector<string> GetSections() {
		vector<string> mapped;

		for (auto outer = ItemMap.begin(); outer != ItemMap.end(); ++outer) {
			auto vit = Find(mapped, outer->first.first);
			if (!vit) {
				mapped.push_back(outer->first.first);
			}
		}

		return mapped;
	}

	vector<pair<string, string>> GetDataFromSection(const char* pSection) {
		vector<pair<string, string>> mapped;

		for (auto outer = ItemMap.begin(); outer != ItemMap.end(); ++outer) {
			if (!strcmp(outer->first.first.c_str(), pSection)) {
				mapped.push_back(make_pair(outer->first.second, outer->second));
			}
		}

		return mapped;
	}

	int ReadInt(const char* pSection, const char* pWhat, int defaultVal) {
		if (!ItemMap[make_pair(pSection, pWhat)].empty()) {
			DbgPrint("Reading from config: [%s] -> %s", pSection, pWhat);
			return stoi(ItemMap[make_pair(pSection, pWhat)].c_str());
		}
		
		return defaultVal;
	}

	const char* ReadString(const char* pSection, const char* pWhat) {
		DbgPrint("Reading from config: [%s] -> %s", pSection, pWhat);
		return ItemMap[make_pair(pSection, pWhat)].c_str();
	}

	float ReadFloat(const char* pSection, const char* pWhat, float defaultVal) {
		if (!ItemMap[make_pair(pSection, pWhat)].empty()) {
			return atof(ItemMap[make_pair(pSection, pWhat)].c_str());
		}

		return defaultVal;
	}

	bool ReadBool(const char* pSection, const char* pWhat, bool def) {
		if (!ItemMap[make_pair(pSection, pWhat)].empty()) {
			DbgPrint("Reading from config: [%s] -> %s", pSection, pWhat);
			const char* ret = ItemMap[make_pair(pSection, pWhat)].c_str();
			if (strstr(ret, "yes") || strstr(ret, "YES")
				|| strstr(ret, "true") || strstr(ret, "TRUE")
				|| strstr(ret, "1")) {
				return true;
			}
		}

		return def;
	}

	GColor ReadColor(const char* pSection, const char* pWhat, GColor def) {
		if (!ItemMap[make_pair(pSection, pWhat)].empty()) {
			DbgPrint("Reading from config: [%s] -> %s", pSection, pWhat);
			vector<string> splitVals;
			pUtils.Split(ItemMap[make_pair(pSection, pWhat)], ',', splitVals);
			
			if (splitVals.size() == 4) {
				return GColor(
					atof(splitVals[0].c_str()) * 255,
					atof(splitVals[1].c_str()) * 255,
					atof(splitVals[2].c_str()) * 255,
					atof(splitVals[3].c_str()) * 255,
					false
				);
			} else {
				DbgPrint("tried reading color but size was %i and not 4", splitVals.size());
			}
		}

		return def;
	}

	bool IsGood() {
		return bIsGood;
	}

	map<pair<string, string>, string> ItemMap;
private:
	bool bIsGood;

	bool Find(vector<string> mapped, string val) {
		for (int i = 0; i < mapped.size(); i++) {
			if (mapped[i] == val)
				return true;
		}
		return false;
	}
};

class Config {
public:
	string pCreateConfigName;
	char szCreateConfigNameBuffer[0x50];
	bool bCreateConfigExt;

	string pLoadConfigName;
	char szLoadConfigNameBuffer[0x50];
	bool bLoadConfigExt;

	bool bCanLoadConfigOnceInitialized;
	bool bLoadCurrentSettings;
	bool bSaveCurrentSettings;

	bool bShouldShowChangelog;
	char szChangelogData[0x300];
	int iServerChangelogVersion;

	void Initialize();
	void UpdateCurrentSettings();
	void CreateConfig(const char* pFile);
	void LoadConfig(const char* pFile);
};

extern Config pConfig;