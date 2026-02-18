namespace GOTHIC_NAMESPACE
{
	namespace Internals
	{
		template <typename T>
		concept Archiveable = requires(T e, zCArchiver & arc)
		{
			T{};
			e.Archive(arc);
			e.Unarchive(arc);
		};

		class SaveableBase
		{
		protected:
			SaveableBase(const char* name);
			zSTRING GetPath() const;

			std::string name;

		private:
			void Save();
			void Load();

			virtual void Archive(zCArchiver& arc) = 0;
			virtual void Unarchive(zCArchiver& arc) = 0;
			virtual void Clear() = 0;

			static void __fastcall OnSave();
			static void OnNewGame();
			static void __fastcall OnLoad();

			static std::list<SaveableBase*> saveables;
		};
	}

	template <Internals::Archiveable T>
	class Saveable : public Internals::SaveableBase
	{
	public:
		Saveable(const char* name);
		Saveable(const Saveable&) = delete;
		Saveable& operator=(const Saveable&) = delete;

		T* operator->();
		const T* operator->() const;
		T& operator*();
		const T& operator*() const;

	private:
		virtual void Archive(zCArchiver& arc) override;
		virtual void Unarchive(zCArchiver& arc) override;
		virtual void Clear() override;

		T* data;
	};

#pragma region Implementation

	namespace Internals
	{
		SaveableBase::SaveableBase(const char* name)
		{
			if (!STD_ENGINE_CHECKING)
				return;

			this->name = name;
			std::transform(this->name.begin(), this->name.end(), this->name.begin(), ::toupper);
			saveables.push_back(this);

			if (saveables.size() != 1)
				return;

			Union::CreatePartialHook(std::bit_cast<void*>(ZENDEF(0x0063ADAD, 0x006616AD, 0x006685FD, 0x006C527D)), OnSave);
			new GameSub<void>{ GameEvent::LoadBegin_NewGame, OnNewGame };
			Union::CreatePartialHook(std::bit_cast<void*>(ZENDEF(0x0063C5BA, 0x006630D7, 0x00669EBA, 0x006C6AEA)), OnLoad);
		}

		zSTRING SaveableBase::GetPath() const
		{
			constexpr int SAVEGAME_SLOT_CURRENT = -1;

			zSTRING path = zoptions->GetDirString(DIR_ROOT) + zoptions->GetDirString(DIR_SAVEGAMES) +
				ogame->savegameManager->GetSlotDirName(SAVEGAME_SLOT_CURRENT);

			path += name.c_str();
			path += ".SAV";
			return path;
		}

		void SaveableBase::Save()
		{
			std::unique_ptr<zFILE> file{ zfactory->CreateZFile(GetPath()) };
			file->DirCreate();
			file->Create();

			zPtr<zCArchiver> arc{ zarcFactory->CreateArchiverWrite(file.get(), zARC_MODE_ASCII, true, 0) };
			Archive(*arc);
			arc->Close();
		}

		void SaveableBase::Load()
		{
			Clear();

			if (zPtr<zCArchiver> arc{ zarcFactory->CreateArchiverRead(GetPath(), 0) })
			{
				Unarchive(*arc);
				arc->Close();
			}
		}

		void __fastcall SaveableBase::OnSave()
		{
			for (SaveableBase* saveable : saveables)
				saveable->Save();
		}

		void SaveableBase::OnNewGame()
		{
			for (SaveableBase* saveable : saveables)
				saveable->Clear();
		}

		void __fastcall SaveableBase::OnLoad()
		{
			for (SaveableBase* saveable : saveables)
				saveable->Load();
		}

		std::list<SaveableBase*> SaveableBase::saveables;
	}

	template <Internals::Archiveable T>
	Saveable<T>::Saveable(const char* name) :
		SaveableBase{ name },
		data{}
	{

	}

	template <Internals::Archiveable T>
	T* Saveable<T>::operator->()
	{
		return data;
	}

	template <Internals::Archiveable T>
	const T* Saveable<T>::operator->() const
	{
		return data;
	}

	template <Internals::Archiveable T>
	T& Saveable<T>::operator*()
	{
		return *data;
	}

	template <Internals::Archiveable T>
	const T& Saveable<T>::operator*() const
	{
		return *data;
	}

	template <Internals::Archiveable T>
	void Saveable<T>::Archive(zCArchiver& arc)
	{
		data->Archive(arc);
	}

	template <Internals::Archiveable T>
	void Saveable<T>::Unarchive(zCArchiver& arc)
	{
		data->Unarchive(arc);
	}

	template <Internals::Archiveable T>
	void Saveable<T>::Clear()
	{
		delete data;
		data = new T{};
	}

#pragma endregion
}