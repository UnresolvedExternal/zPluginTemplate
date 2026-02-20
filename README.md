# Introduction

This project serves as a starting point for creating a union plugin for the following Gothic games:
- [Gothic I](https://gothic.fandom.com/wiki/Gothic_1)
- [Gothic Sequel](https://gothic.fandom.com/wiki/Gothic_Sequel)
- [Gothic II](https://en.wikipedia.org/wiki/Gothic_II)
- [Gothic II Night of The Raven](https://en.wikipedia.org/wiki/Gothic_II:_Night_of_the_Raven)

It provides a preconfigured base code designed to simplify the development process and help you focus on building new features for your plugin.

# Requirements

Before you start making your own plugin, you need to install some software first, here's a full list of things that you'll need to install to be able to build the union plugin:
- [git](https://git-scm.com/) **Required** for version control and to clone the project repository
- [CMake](https://cmake.org/) **Optional** if you plan to use Visual Studio
- [Visual Studio](https://visualstudio.microsoft.com/pl/) **Essential** for compiling the plugin using the MSVC toolset  
	(make sure to install **C++ Workload** and **CMake Tools for Visual Studio**)
- [Hook Snippets](https://drive.google.com/file/d/1nJF2BbEE-sN3Wa3A99D4lqqZ4HxPA4_R/view?usp=drive_link) in order to place hooks quickly
	(unpack it to **%userprofile%/Documents/Visual Studio 2022/Code Snippets/Visual C++/**)

The Union plugin requires the MSVC toolset for compatibility, so alternative toolchains like MinGW are not supported.

# Fetching the source code

1. Make sure to clone the your project repository recursively (to fetch all of the [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules)).  
2. You can achieve this by typing this command in your terminal: 
```git 
git clone --recursive URL_TO_YOUR_REPO
```

# Configuration

The plugin configuration is located in **CMakeLists.txt**.
The name of your project is the name of the consisting directory.

Some of the common things that you should propably change are:
- **project version** (in **CMakeLists.txt**)  
	default value is **1.0.0.0**

- paths to Gothic executeables (5 places in **CMakePresets.json**)  
	default values are  
		**D:/Gothic/g1/System/GothicMod.exe** for **G1**  
		**D:/Gothic/g1a/System/GothicMod.exe** for **G1A**  
		**D:/Gothic/g2/System/Gothic2.exe** for **G2**  
		**D:/Gothic/g2a/System/Gothic2.exe** for **G2A**  
		**D:/Gothic/g2a/System/Gothic2.exe** for **MP**

# Building

Follow the steps below to compile the plugin:

## Step 1: Open the Project in Visual Studio

1. Navigate to the root directory of your repository.
2. Right-click on the folder's content (without selecting any files or subfolders).
3. Select **Open with Visual Studio** from the context menu.

## Step 2: Choose a Configuration

1. In Visual Studio, locate the Solution Configurations dropdown menu in the top toolbar.
2. Select the desired configuration for your build
	default configuration is **G2A-Debug**

## Step 3: Pick the Startup Project

1. In Visual Studio set **CMakeLists.txt** as a startup element

### Step 4: Build the plugin

1. Once everything is configured, click **Build CMakeLists.txt** (or press **Ctrl+B**).
2. If you've configured everything correctly, the build process should complete successfully.
3. If you provided correct Gothic executable path in **CMakePresets.json** then the generated VDF volume will be copied to **Data/Plugins** subfolder.
	If Union is installed you are able to run Gothic with the plugin from Visual Studio via **F5** or **Ctrl+F5** (with / without debugger)

# Publishing plugin

This project provides [github action](https://github.com/features/actions) and [gitlab pipeline](https://docs.gitlab.com/ci/yaml/) for compiling and releasing a new version of your plugin.  

Before you publish a new release, make sure to set a new version in **CMakeLists.txt**, and document your changes in **CHANGELOG.md** file. I recommend updating your changelog file regularly during the development of your plugin, to not forget about adding this later.

To publish a new version of your plugin you just need to create a new [github release](https://docs.github.com/en/repositories/releasing-projects-on-github/managing-releases-in-a-repository) or [gitlab release](https://docs.gitlab.com/user/project/releases/) or push a [git tag](https://git-scm.com/book/en/v2/Git-Basics-Tagging).  
I recommend naming your release by using your plugin version.

And that's it, when plugin will be built successfully it will automatically be added as release asset to the newest release. By default CI/CD script is using the **MP-Release** configuration, depending on your plugin requirements you might want to change this, to match your plugin supported platform(s).