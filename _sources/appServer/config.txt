.. _configAppServer:

Configuracion AppServer
=======================

La configuración del shared server se puede hacer de distintas formas. Si ninguna de estas es utilizada iniciara con valores predeterminados de puerto, link al shared server y configuración de los logs. 

El server primero busca el archivo server.conf en el mismo directorio del proyecto para cargar los datos, este debe tener dentro solo los parametros en formato JSON. Las claves son: "port"(número de puerto),"shared"(link al shared server) y "logfile"(direccion del archivo de configuración).

Ejemplo de archivo server.conf:

.. code-block:: javascript

	{
		"port":8000,
		"shared":"http://dr-tinder.herokuapp.com",
		"logfile”:"logCnfs/myLog.conf"
	}	

Si alguno de estos valores falta se tomará el default:

* Port : 8000
* Link al shared server : http://dr-tinder.herokuapp.com
* Log file : logsCnfs/default.conf

 A su vez si se corriera localmente podríamos sobrepasar alguno o todos los parametros si al correr el server le pasaramos las siguientes opciones:

.. code-block:: bash

	-p
	--port
			Número de puerto
	-s
	--shared 
			Link al shared Server
	-l
	--log
			Tipo de log (actualmente toma debug, sino toma el valor predeterminado)
	-lf
	--logfile
			Direccion al archivo de configuracion (ignora el flag -l)

Ejemplo:

.. code-block:: bash

	./server -p 8000 -s http://localhost:5000 -l debug

*****
Logs
*****

Para los logs del server se utilizó `easylogging++ <https://github.com/easylogging/easyloggingpp>`_. Este permite configurar que tipos de log son mostrados en el standard output y en que formato. Dentro del proyecto en el directorio **logCnfs/** se encuentran actualmente dos archivos de configuración de estos. El server toma el *default.conf* como archivo para sus configuración. Si desea agregar una nueva configuración cree el archivo respetando la documentación de easylogging++ y indique su direccion en el archivo de *server.conf* o paselo como parametro con -lf  *o*  --logfile.

