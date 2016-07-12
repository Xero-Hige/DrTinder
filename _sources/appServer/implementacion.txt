.. _implementacion:

Implementacion AppServer
========================

Este sistema tiene principalmente tres partes:

#. Recepción y procesamiento de mensajes HTTP
#. Conexión al Shared Server
#. Almacenamiento de datos

******************************************
Recepción y procesamiento de mensajes HTTP
******************************************

Para la  conexión se utilizo mongoose web server de Cesanta ( `<https://github.com/cesanta/mongoose>`_ ).
Mediante este se reciben y procesan. El **RequestHandler** toma el mensaje y decide cómo procesarlo diferenciando la uri a la cual se hizo y con qué método se lo llamó. Este llama a un **MessageHandler** que se ocupa del procesamiento de tal mensaje, almacenando, parseando los datos y llamando al shared server.

**************************
Conexión al Shared Server
**************************

Para la conexión al shared server se utilizo la libreria de curl, para poder correr el server debe tenerlo instalado. Para no profundizar en cómo usar curl se uso restclient-cpp ( `<https://github.com/mrtazz/restclient-cpp>`_ ) que permite hacer los requests de manera simple.
Se debió crear un CsvParser y JsonParser (libreria utilizada: `jsoncpp <https://github.com/open-source-parsers/jsoncpp>`_ ) para poder traducir la información que venía del shared server para pasarla al cliente.

***********************
Almacenamiento de Datos
***********************

De los usuarios era necesario guardar nuevos datos en el server como la contraseña y una descripcion de estos, ademas de los token de autenticación, los likes entre ellos y sus respectivos chats. Para el almacenamiento de estos se utilizó rocksdb ( `<http://rocksdb.org/>`_ ), que es una base de datos NoSql. Se crearon el DatabaseManager, CharDatabaseManager y LikesDatabaseManager para abstraernos un poco de su implementación específica.
Para guardar los datos del usuario se utilizan prefijos dependiendo del dato, por ejemplo para guardar la descripción se utiliza el prefijo *LOOKING_FOR_DB* al cual se concatena el mail de usuario para tener de clave y como valor se guarda la descripcion. 

Se utilizan tres bases de datos:
	- para password, id, token de user
	- para chats de user
	- para likes y matches de user
