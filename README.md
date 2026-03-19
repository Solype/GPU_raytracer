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
   git clone https://github.com/ton-utilisateur/opencl-raytracer.git
   cd opencl-raytracer
  ```
2. Crée un dossier `build` et compile avec CMake :
  ```bash
   mkdir build
   cd build
   cmake ..
   make
  ```

---

## 📂 Structure du projet


| Dossier     | Description                                                        |
| ----------- | ------------------------------------------------------------------ |
| `/src`      | Code source du raytracer (C++/C)                                   |
| `/include`  | Fichiers d'en-tête (.h)                                            |
| `/opencl`   | Kernels OpenCL (.cl) pour le rendu sur GPU                         |
| `/cmake`    | Modules CMake pour la configuration                                |
| `/examples` | Exemples de scènes et de configurations                            |
| `/docs`     | Documentation et ressources pour apprendre OpenCL et le raytracing |


---

## 🚀 Utilisation

1. **Exécuter le raytracer** :
  ```bash
   ./raytracer
  ```
2. **Personnaliser une scène** :
  - Modifiez les fichiers dans `/examples` pour créer vos propres scènes.
  - Adaptez les kernels OpenCL dans `/opencl` pour expérimenter avec le rendu.

---

## 📖 Apprendre OpenCL avec ce projet

### 1. **Comprendre les bases d'OpenCL**

- Lisez la documentation dans `/docs/opencl-basics.md`.
- Étudiez les kernels OpenCL dans `/opencl` pour voir comment les calculs sont parallélisés.

### 2. **Raytracing avec OpenCL**

- Le raytracer utilise OpenCL pour accélérer les calculs de rendu.
- Consultez `/src/raytracer.cpp` pour voir comment les buffers OpenCL sont utilisés.

### 3. **Exercices pratiques**

- Ajoutez de nouvelles fonctionnalités (ex: réflexions, ombres douces).
- Expérimentez avec différents kernels OpenCL pour optimiser les performances.

---

## 🔧 Configuration CMake

Le fichier `CMakeLists.txt` est configuré pour :

- Trouver automatiquement OpenCL sur votre système.
- Compiler les kernels OpenCL et les lier au projet.
- Générer un exécutable pour le raytracer.

---

## 📝 Licence

Ce projet est sous licence **MIT**. Consultez le fichier `LICENSE` pour plus de détails.

---

## 📬 Contact

Pour toute question ou suggestion, n'hésitez pas à ouvrir une *issue* ou à me contacter directement.
