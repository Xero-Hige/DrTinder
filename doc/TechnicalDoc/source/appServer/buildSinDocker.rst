.. buildSinDocker:

Build AppServer sin Docker
==========================

Para buildear el server es necesario tener instalado todas las librerías mencionadas en la implementación y CMake (*version mayor o igual a 2.8*). Para ello clone el repositorio y ejecute los siguientes comandos:

.. code-block:: bash

	apt-get update 
	apt-get install -y  autoconf cmake curl  g++  libbz2-dev  libcurl4-openssl-dev  libgflags-dev   libsnappy-dev libssl-dev libtool make openssl python zlib1g-dev
	git submodule init
	git submodule update

DrTinder/source/libs/rocksdb:

.. code-block:: bash

	make install

DrTinder/source/libs/jsoncpp:

.. code-block:: bash

	python amalgamate.py

DrTinder/source/libs/googletest :

.. code-block:: bash

	cmake .
	make install

DrTinder/source/libs/restclient-cpp:

.. code-block:: bash

	./autogen.sh 
	./configure 
	make install

DrTinder/source/libs/swift:

.. code-block:: bash

	./scons SWIFTEN_INSTALLDIR=/usr/local /usr/local
	apt-get install -y libboost-all-dev

Luego de instalados para buildear test y servers:

DrTinder/source/Server:

.. code-block:: bash

	cmake .
	make

*****
Tests
*****
Para los tests se utilizaron librerias de python (**unittest** y **requests**) como de c++ (`gtest <https://github.com/google/googletest>`_ ).
Los tests de python apuntan a verificar el correcto procesamiento de los requests que se le pueden hacer al server y los de c++ a probar la funcionalidad particular de las clases creadas(unit test).
Para poder llevar una medida de lo que abarcan estos tests se utilizó gcov para medir el coverage. Tambien se utilizo lcov ( `<https://github.com/linux-test-project/lcov>`_ ) para poder visualizar el coverage de mejor forma.

Coverage
########

Para visualizar el coverage debe primero buildear los tests y el server en modo Debug, para ello debe correr primero:

.. code-block:: bash

	cmake -DCMAKE_BUILD_TYPE=Debug
	make

Luego corra los tests con :

.. code-block:: bash

	./Test

Después levante el server y dejelo corriendo en una terminal. Abra otra terminal y ejecute los test de Python ( DrTinder/source/Servidor/tests/testPythonAppApiTest.py ).

Luego cierre el server escribiendo *quit* en su respectiva terminal. Una vez cerrado corra *./coverage.sh*. Este script dejará en la carpeta coverage unos archivos entre los cuales se encuentra un *index.html* que muestra el nivel de coverage de los distintos archivos.

