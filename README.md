# OpenCL Raytracer Project

**Un projet pour apprendre à utiliser OpenCL dans un raytracer en C++/C.**

Ce dépôt contient un raytracer simple implémenté en C++/C, utilisant OpenCL pour accélérer les calculs sur GPU. Il est conçu pour apprendre à utiliser OpenCL dans un contexte de rendu graphique.

---

## 📌 Prérequis

- **Compilateur C++** (GCC, Clang, MSVC)
- **CMake** (version 3.10 ou supérieure)
- **SDK OpenCL** compatible avec votre matériel (ex: [Intel OpenCL SDK](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html), [NVIDIA CUDA Toolkit](https://developer.nvidia.com/cuda-downloads), [AMD ROCm](https://rocm.docs.amd.com/))
- **Git** (pour cloner le dépôt)

---

## 🛠 Installation

1. Clone ce dépôt :
  ```bash
   git clone https://github.com/Solype/GPU_raytracer
   cd GPU_raytracer
  ```
2. Crée un dossier `build` et compile avec CMake :
  ```bash
   mkdir build
   cd build
   cmake ..
   make
  ```

---

## 🚀 Utilisation

1. **Exécuter le raytracer** depuis le dossier `build` (le kernel est chargé depuis `./kernel_gpu`) :
  ```bash
   ./app              # scène finale de "Ray Tracing in One Weekend" (1200px, 500 spp)
   ./app materials    # diffus, métal, verre creux, flou de profondeur (400px, 100 spp)
   ./app --help
  ```
  L'image est écrite dans `image.ppm`.
2. **Personnaliser une scène** :
  - Ajoutez une fonction `scene_xxx` dans `main.cpp` (primitives + réglages caméra) et enregistrez-la dans la table `scenes` de `main`.
  - Adaptez les kernels OpenCL dans `kernel_gpu/` pour expérimenter avec le rendu.

---

## 📖 Apprendre OpenCL avec ce projet

### 1. **Comprendre les bases d'OpenCL**

- Étudiez `GPUClasses/` : un wrapper C++ minimal autour de l'API OpenCL (plateforme, device, contexte, programme, kernel, queue).
- Étudiez les kernels OpenCL dans `kernel_gpu/` (point d'entrée : `entry_point.c`, kernel : `compute_ray` dans `main.cl`) pour voir comment les calculs sont parallélisés : un work-item par pixel.

### 2. **Raytracing avec OpenCL**

- Le raytracer utilise OpenCL pour accélérer les calculs de rendu.
- Consultez `main.cpp` pour voir comment les buffers OpenCL sont créés, passés au kernel et relus.
- Les structures partagées entre l'hôte et le GPU sont dans `kernel_gpu/struct.h`.

### 3. **Exercices pratiques**

- Ajoutez de nouvelles fonctionnalités (ex: réflexions, ombres douces).
- Expérimentez avec différents kernels OpenCL pour optimiser les performances.

---

## 🔧 Configuration CMake

Le fichier `CMakeLists.txt` est configuré pour :

- Trouver automatiquement OpenCL sur votre système.
- Générer un exécutable pour le raytracer.
- Copier `kernel_gpu/` dans le dossier de build à chaque `make` : les kernels sont compilés par OpenCL au lancement de `./app`.

---

## 📬 Contact

Pour toute question ou suggestion, n'hésitez pas à ouvrir une *issue* ou à me contacter directement.
