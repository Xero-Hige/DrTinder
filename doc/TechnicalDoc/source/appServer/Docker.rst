.. _docker:

Docker build
============

El despliegue del App Server se realiza a través de **Docker**, por lo que a continuación se enumeran los pasos necesarios para poder deployar el servidor de forma exitosa:

***********
Instalación
***********

Instalación de Docker
#####################

Para poder deployar el servidor, es necesario contar con una instalación de Docker. Para ver las instrucciones para su instalación haga click `aqui <https://docs.docker.com/engine/installation/>`_.

Instalación del Servidor
########################

Descargar el archivo Dockerfile del `repositorio <https://github.com/Xero-Hige/DrTinder/blob/AppServer/docker-server-run/Dockerfile>`_.

Pararse en el directorio donde se encuentra el Dockerfile.

Ejecutar el siguiente comando:

.. code-block:: bash

	docker build --no-cache -t docker-server .

Esto deja creada una imagen de docker del servidor que puede ser ejecutada en cualquier momento.

****************
Puesta en marcha
****************

Para poder deployar el servidor, se debe correr la imagen de Docker creada previamente. Para esto, se debe ejecutar el comando:

.. code-block:: bash

	docker run -p 8080:80 docker-server

Esto dejará corriendo el servidor en el puerto 8080 de la máquina sobre la que se está trabajando.

********************
Apagado del servidor
********************

Para apagar el servidor, ejecutar el siguiente comando para ver los containers de docker activos:

.. code-block:: bash

	docker ps

El comando devuelve algo similar a esto:

.. code-block:: bash

	CONTAINER ID        IMAGE           ...   NAMES
	1a9363e0906c        docker-server   ...   boring_jennings

Para frenar el container, se debe ejecutar el comando:

.. code-block:: bash

	docker kill <nombre_del_container>

El *<nombre_del_container>* se encuentra en la columna **NAMES** de la tabla anterior. En este caso el comando sería:

.. code-block:: bash

	docker kill boring_jennings
