<h1 align="center">Particle Physics</h1>

<div align="center">
    <a href="https://sonarcloud.io/summary/new_code?id=xorz57_ParticlePhysics">
        <img src="https://sonarcloud.io/api/project_badges/measure?project=xorz57_ParticlePhysics&metric=alert_status" alt="Quality Gate Status">
    </a>
    <a href="https://sonarcloud.io/summary/new_code?id=xorz57_ParticlePhysics">
        <img src="https://sonarcloud.io/api/project_badges/measure?project=xorz57_ParticlePhysics&metric=bugs" alt="Bugs">
    </a>
    <a href="https://sonarcloud.io/summary/new_code?id=xorz57_ParticlePhysics">
        <img src="https://sonarcloud.io/api/project_badges/measure?project=xorz57_ParticlePhysics&metric=vulnerabilities" alt="Vulnerabilities">
    </a>
    <a href="https://sonarcloud.io/summary/new_code?id=xorz57_ParticlePhysics">
        <img src="https://sonarcloud.io/api/project_badges/measure?project=xorz57_ParticlePhysics&metric=code_smells" alt="Code Smells">
    </a>
    <a href="https://sonarcloud.io/summary/new_code?id=xorz57_ParticlePhysics">
        <img src="https://sonarcloud.io/api/project_badges/measure?project=xorz57_ParticlePhysics&metric=coverage" alt="Coverage">
    </a>
    <a href="https://sonarcloud.io/summary/new_code?id=xorz57_ParticlePhysics">
        <img src="https://sonarcloud.io/api/project_badges/measure?project=xorz57_ParticlePhysics&metric=duplicated_lines_density" alt="Duplicated Lines (%)">
    </a>
</div>

## Table of Contents

- [Work in Progress](#work-in-progress)
- [Dependencies](#dependencies)
- [Getting Started](#getting-started)
- [License](#license)
- [Acknowledgments](#acknowledgments)
- [Contact](#contact)

## Work in Progress

🚧 **Work in Progress Repository** 🚧

📘 **Purpose:** Learning and improving skills.

🛠️ **Status:** Work in progress, not aiming for perfection.

🔍 **Goals:** Explore, learn, and grow.

📚 **Join:** We welcome contributions and feedback.

🤓 **Note:** Embrace the learning journey. Your input is valued!

## Dependencies

Engine relies on the following libraries:

- [spdlog](https://github.com/gabime/spdlog)
- [glm](https://github.com/g-truc/glm)
- [SFML](https://github.com/SFML/SFML)
- [ImGui-SFML](https://github.com/SFML/imgui-sfml)
- [implot](https://github.com/epezent/implot)

Please ensure you have these dependencies installed before building the project.

## Getting Started

#### Linux & macOS

```bash
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh

git clone https://github.com/xorz57/ParticlePhysics.git
cd ParticlePhysics
cmake -B build -DCMAKE_BUILD_TYPE=Release -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
ctest --build-config Release
```

#### Windows

```powershell
git clone https://github.com/microsoft/vcpkg.git C:/vcpkg
C:/vcpkg/bootstrap-vcpkg.bat
C:/vcpkg/vcpkg.exe integrate install

git clone https://github.com/xorz57/ParticlePhysics.git
cd ParticlePhysics
cmake -B build -DCMAKE_BUILD_TYPE=Release -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
ctest --build-config Release
```

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

We would like to express our gratitude to the authors and maintainers of the libraries and tools that Engine depends on. Their work is greatly appreciated.

## Contact

For questions, suggestions, or feedback, feel free to contact us:

- Email: [xorz57@gmail.com](mailto:xorz57@gmail.com)
- GitHub: [xorz57](https://github.com/xorz57)
