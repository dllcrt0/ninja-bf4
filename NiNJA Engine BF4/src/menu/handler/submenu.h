#pragma once

class Option;

class Submenu {
protected:
	vector<shared_ptr<Option>> Options;
	const char* pName;
	Submenu* pParentSubmenu;

public:
	int iOldCurrentOption;
	int iOldScrollOffset;

	Submenu() : pName(""), pParentSubmenu(nullptr), iOldCurrentOption(0), iOldScrollOffset(0) {};

	int GetOptionAmount() {
		int size = 0;

		for (auto i = 0; i < Options.size(); i++) {
			if (Options.at(i)->IsVisible()) {
				size++;
			}
		}

		return size;
	}

	void MenuUpdate();

	virtual void Init(); // Option setup
	virtual void UpdateOnce(); // Update once when submenu entered
	virtual void Update(); // Update while submenu active
	virtual void FeatureUpdate(); // Background Update
	virtual void InitVars(); // init vars

	template<typename T>
	shared_ptr<T> addOption(T& option) {
		shared_ptr<T> ptr = make_shared<T>(option);
		Options.push_back(ptr);
		return ptr;
	}

	template<typename T>
	void addOption(shared_ptr<T>& option) {
		Options.push_back(option);
	}

	void SetName(const char* name) {
		pName = name;
	}

	template<typename T>
	void SetParentSubmenu() {
		pParentSubmenu = T::GetInstance();
	}

	vector<shared_ptr<Option>> GetOptions() {
		return Options;
	}

	const char* GetName() { 
		return pName; 
	}

	Submenu* GetParentSubmenu() { 
		return pParentSubmenu; 
	}

	void ClearOptionsOffset(int offset) {
		Options.resize(offset);
	}
};