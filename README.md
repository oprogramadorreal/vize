![vize](/vize/vize.jpg)
## VIZE - Visualização Interativa de Volumes

Para uma introdução a este projeto, assista ao vídeo: https://youtu.be/EEhfCa1iP8k

### Features
- Exclusivo modo de visualização interativo com ray casting e MPR simultâneamente;
- Suporta imagens DICOM e TIFF;
- Visualização de histograma;
- Intuitivo editor de função de transferência;
- Implementação em GPU com GLSL;
- Código:
	- C++ moderno;
	- OpenGL moderno;
	- 100% código aberto!

### Visão geral
No diretório raiz estão 3 sub-projetos: **3rd-party**, **epoch** e **vize**.

**3rd-party** contém bibliotecas de terceiros. São dependencias externas, que não foram desenvolvidas por mim. **epoch** é o nome do projeto que contém as bibliotecas básicas, desenvolvidas por mim, e que são usadas pelo terceiro sub-projeto, o **vize**.

Portanto, tudo que existe em **3rd-party** e **epoch** geram bibliotecas e o **vize** gera o executável que usa essas biblitecas.

### Ray casting em GPU

A implementação do ray casting em GPU tem algumas sacadas (bem expertinhas) para tirar proveito da arquitetura extremamente paralela desses processadores.

A classe que configura e dispara o ray casting está no projeto **vize** e se chama *VolumePainter*. O método chama-se *_paintImpl*. Mas isso ainda é C++ e roda na CPU.

O código da implementação do ray casting que roda na GPU está nos arquivos *mpr_ray_casting.fs* e *mpr_ray_casting.vs* que estão em *vize/resources/shaders*.

------------

### Como Compilar (em Windows)

#### 1. Baixe e instale os softwares necessários
1. **Git**: https://git-scm.com/downloads
2. **Visual Studio Community**: https://visualstudio.microsoft.com/
3. **CMake**: https://cmake.org/download/

#### 2. Baixe e instale as dependências

1. **Boost**:
https://sourceforge.net/projects/boost/files/boost-binaries/1.71.0/boost_1_71_0-msvc-14.2-64.exe/download (versão sugerida)

2. **Qt**:
https://download.qt.io/archive/qt/5.13/5.13.2/qt-opensource-windows-x86-5.13.2.exe (versão sugerida)

#### 3. Configure as variáveis de ambiente

Para que o **CMake** possa encontrar corretamente as bibliotecas instaladas no passo anterior (**Boost** e **Qt**), você deve definir as seguintes variáveis de ambiente no seu sistema:

1. **QTDIR**: {LocalOndeVocêInstalouOQt}\5.13.2\msvc2017_64
2. **BOOST_INCLUDEDIR**: {LocalOndeVocêInstalouABoost}
3. **BOOST_LIBRARYDIR**: {LocalOndeVocêInstalouABoost}\lib64-msvc-14.2
4. **BOOST_ROOT**: {LocalOndeVocêInstalouABoost}\boost

#### 4. Clone o repositório

git clone git@github.com:oprogramadorreal/vize.git

#### 5. Gere o projeto do Visual Studio

Para gerar a "solution" do Visual Studio para este projeto usando o CMake, basta rodar o script **build-vc16-win64.bat** que está em *vize/build/*.

Ao fazer isso, será gerado o arquivo **vize.sln** em *vize/build/vc16-win64/*. Pronto! Agora, basta abri-lo com o Visual Studio e mandar compilar.