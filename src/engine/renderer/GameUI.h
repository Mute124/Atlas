#pragma once
#include "../core/Common.h"
#include <string>
#include <type_traits>

#ifdef ATLAS_ENABLE_HTML_UI

#include <RmlUi/Config/Config.h>
#include <RmlUi/Core.h>
#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Math.h>
#include <RmlUi/Core/RenderInterface.h>
#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Vertex.h>
#include <RmlUi/Debugger.h>

#endif // ATLAS_ENABLE_HTML_UI

#include <raygui.h>

namespace Atlas {
#ifdef ATLAS_ENABLE_HTML_UI
	using UIVertex = Rml::Vertex;
	using FileHandle = Rml::FileHandle;
	using Rml::TextureHandle;

	
	/// \concept FileInterface
	/// \brief Ensures that the type is a child of Rml::FileInterface because IGameUI has a template parameter that requires a child of Rml::FileInterface, so this just ensures the rules are followed.
	template<typename T>
	concept FileInterfaceType = std::convertible_to<T*, Rml::FileInterface*>;


	class GameUIRenderer : public Rml::RenderInterface {
	public:
		

		// Inherited via RenderInterface
		void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;

		void EnableScissorRegion(bool enable) override;

		void SetScissorRegion(int x, int y, int width, int height) override;

	};


	class GameUIFileInterface : public Rml::FileInterface {
	public:
		explicit GameUIFileInterface(const std::string& root);
		~GameUIFileInterface() override;

		FileHandle open(const string& path) { return Open(path); }
		void close(FileHandle file) { Close(file); }
		size_t read(void* buffer, size_t size, FileHandle file) { return Read(buffer, size, file); }
		bool seek(FileHandle file, long offset, int origin) { return Seek(file, offset, origin); }
		size_t tell(FileHandle file) { return Tell(file); }

		string mRootPath;

		// These functions are private because they will be wrapped by functions that follow code conventions
		Rml::FileHandle Open(const std::string& path) override;
		void Close(Rml::FileHandle file) override;
		size_t Read(void* buffer, size_t size, Rml::FileHandle file) override;
		bool Seek(Rml::FileHandle file, long offset, int origin) override;
		size_t Tell(Rml::FileHandle file) override;
	};

	class UISystemInterface : public Rml::SystemInterface {
	public:
		double GetElapsedTime() override {
			static const auto start = std::chrono::steady_clock::now();
			const auto current = std::chrono::steady_clock::now();
			std::chrono::duration<double> diff = current - start;
			return diff.count();
		}



	};
	template<FileInterfaceType T>
	class IGameUI abstract {
	public:
		IGameUI() = default;
		virtual ~IGameUI() = default;

		virtual GameUIFileInterface* getFileInterface() = 0;
		virtual UISystemInterface* getSystemInterface() = 0;
		virtual void init() = 0;
	};


	class GameUI : public IGameUI<Rml::FileInterface> {
	public:
		GameUIFileInterface* getFileInterface() override;
		UISystemInterface* getSystemInterface() override { return mSystemInterface; }

		void init() override;

	private:
		UISystemInterface* mSystemInterface = new UISystemInterface();
		GameUIFileInterface* mFileInterface = new GameUIFileInterface(ATLAS_ASSET_DIR);
	};

	template<FileInterfaceType T>
	void InitUI(GameUIRenderer* uiRenderer, IGameUI<T>* ui) {
		ui->init();
		Rml::SetFileInterface(ui->getFileInterface());
		Rml::SetSystemInterface(ui->getSystemInterface());
		Rml::SetRenderInterface(uiRenderer);
		Rml::Initialise();
	}
#endif // ATLAS_ENABLE_HTML_UI

	class IGUIContainer {
	public:

		virtual void LoadStyle(std::string const& stylesheetFileName) = 0;
		
		
		int a = 0;
	};

}