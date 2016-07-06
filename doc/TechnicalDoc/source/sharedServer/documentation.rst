.. _document_Shared:

Documentacion
==============

El shared server se encuentra actualmente en **heroku**. Para seguir manteniéndolo solo hay que pushear al repositorio, este deployea automáticamente el server en heroku. 

Su desarrollo fue realizado con **nodejs** para correr un servidor con el paquete express que respondiera a una API determinada para crear, modificar y eliminar usuarios y crear intereses. Al mandar y recibir información en formato JSON se utilizaron los paquetes de **body-parser** y **jsonschema** para poder recibir la información en las respuestas HTTP y poder validar su formato.

A su vez este server cuenta con una conexión a una base de datos implementada con PostgreSql, por lo que utiliza el paquete de **pg**.

A su vez dispone de una interfaz gráfica para la administración de los usuarios, esta está implementada con html, javascript y css como sitio web y para su construcción se usan los paquete de node **ejs**.

*****************
Instalación local
*****************

Para poder correr localmente el shared server, es decir desde un puerto local y una base de datos local es necesario tener instalado node, npm y postgres. Primero hay que correr el comando: 

.. code-block:: bash

	sudo apt-get update

Instalación de node
###################

Para instalar corra el comando: 

.. code-block:: bash

	sudo apt-get install nodejs

Para asegurarse que se instaló corra:

.. code-block:: bash

	node --version

Si indica que:

.. code-block:: bash

	-bash: /usr/sbin/node: No such file or directory

Corra:

.. code-block:: bash

	sudo ln -s `which nodejs` /usr/bin/node

Instalación del npm
####################

.. code-block:: bash

	sudo apt-get install npm

Instalación de postgres
#######################

.. code-block:: bash
	
	sudo apt-get install postgresql postgresql-contrib

******************
Creación Rol y DB
******************

Primero para tener la base de datos local es necesario ejecutar el archivo createDB.sql, para ello debe ejecutar lo siguiente siendo *<usrid>* su nombre de usuario. Estara creando un rol y podrá ejecutar el último comando desde su terminal.

.. code-block:: bash

	sudo -i -u postgres
	psql -s -d -P <usrid>  #prompt para setear pass
	createdb <usrid>
	exit

Por último ejecute(una vez creado el rol puede correr este comando para reiniciar la base de datos):

.. code-block:: bash

	psql -f createDB.sql

La definición de la base de datos la puede ver en :ref:`dbDiagram`.

********************
Configuracion (.env)
********************

Para poder correr localmente el shared server se debe escribir un archivo .env que solo contenga en formato “key=value” el puerto(PORT) y la url(DATABASE_URL) a la base de datos de Postgres. Para levantar este archivo y setear las variables se utiliza el paquete  node-env-file. Un ejemplo de archivo sería:

.. code-block:: bash

	PORT = 8000
	DATABASE_URL=postgres://user:pass@localhost:5432/dbname

La composición de la url de la base de datos es la siguiente:

.. code-block:: bash

	postgres:://<userid>:<pass>@localhost:5432/tinder

Donde *<userid>* y *<pass>* corresponden al rol creado.

****************
Puesta en marcha
****************

Primero asegurarse que estén instalado los paquetes:

.. code-block:: bash

	npm install

Luego, localizado en la carpeta del proyecto ejecutar:

.. code-block:: bash

	node index.js -l 

O tambien:

.. code-block:: bash

	node index.js --local

La aplicación se puede cerrar con un SIGINT, el mismo al recibir esta señal cierra el servidor y luego termina con código 0.

