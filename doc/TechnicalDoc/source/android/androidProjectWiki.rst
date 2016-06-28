Wiki
=======

.. toctree::
   :maxdepth: 1

   androidWikiCompilereq
   androidWikiDevelopreqst
   androidWikiExternalLibs


Cliente
===========

El cliente consta de una aplicación para Android(4.3+).

Como ambiente de desarrollo se utilizó Android Studio


Bibliotecas y compilación
--------------------------

Para la implementación del cliente se hizo uso de las siguiente bibliotecas:

* Android SDK
 Provee todas las funcionalidades nativas de Android.
* Android Support Library
 Permite realizar aplicaciones con características de Android 5.0+ orientadas a versiones anteriores.
* Hdodenhof CircleImageView
 Biblioteca externa que adapta imagenes a imagenes circulares.
* Retrofit
 Provee un cliente http restful asi como tambien un json adapter para las consultas
* Getbase FloatingActionButton
 Biblioteca externa para incluir un "Floating action button", boton característico de material design.(Se        permiten crear menus y agregar algunos parametros que no están disponibles en el botón nativo)
* SLF4j
   Permite loggear a un archivo

La compilación está configurada por default gracias a Gradle desde Android Studio.



Arquitectura/Diseño
------------------------

La aplicación se desarrolló en torno al patrón MVC aunque también se utilizaron otros patrones de diseño como Singleton y Proxy.

Activities controlan la interacción con el usuario mientras que Retrofit se encarga de la interacción con el servidor. Además se cuenta con clases intermediarias que transforman y manipulan la información.

La comunicación con el servidor se hace mediante request http restful y con bodies en formato json de ser necesarios.

Si bien existen activities de tanto de registro, como de logeo y configuración, la mayor parte de la interacción ocurre en la activity principal, la que representa el almacenamiento de los archivos de usuario.

El flujo de la aplicación es el siguiente:

.. |interaction| image:: images/interaction.png

|interaction|



Para cada interacción del usuario con la UI que genere una request, hay una llamada a una RequestMaker que se encarga de crear un nuevo thread con la request a realizar el server.
Mientras que en background Retrofit se encarga de adaptar la request, según su tipo, al retornar de la llamada a RequestMaker puede que el cliente se bloquee esperando la respuesta del servidor(ej: Esperando a recibir los archivos de la carpeta seleccionada) o que habilite la interacción con el usuario(Ej: Descargando un archivo).

Retrofit permite adaptar clases internas a requests http restful y json, por lo que hay una capa de abstracción para el intercambio de la información. Todas las request se pueden modelar con clases privadas. El mismo caso se da para las respuestas del servidor, donde un Adapter transforma las Server responses a clases propias para tener un fácil acceso a la información recibida.

Cuando el thread de connexion recibe la server response, se ejecutan sus callbacks. success() o failure() si la connexion fue exitosa o fallida.
En este paso entra en juego NetworkCallbackClass. Dado que las cada respuesta del server significa una actualizacion de la iterfaz de usuario, NetworkCallbackClass permite realizar esa conexion de callback-GUI. Es una interfaz con sus métodos implementados por la Vista, pero llamados por los callbacks de conexión. Por este medio se intercambia la información para adaptar la interfaz gráfica.

Clases Principales
------------------------

Activities
+++++++++++++++++

La aplicación cuenta de 5 activities:
* MainActivity para la pantalla principal.
* LoginActivity para la pantalla de logueo.
* RegisterActivity para la pantalla de registro.
* DriveActivity para la pantalla de drive(visualización de archivos y carpetas, administración de los mismos, etc).
* ConfigurationActivity para la pantalla de configuración.

**DriveActivity**
Permite visualizar el drive, con sus carpetas **FolderCard** y sus archivos **FileCard**. La información de los archivos se almacenan en **FileMetadata** mientras que la ruta de las carpetas se mantiene por **Path**.


Cards
++++++++

Tanto **FileCard** como los **FolderCard** cuentan con clases "Adapters" que se encargan de transformar información en forma de *List* a las cards que se muestran en la pantalla.

**FileCard** tiene como atributo **FileMetadata** donde se alamacena la informacion del archivo::


  public class FileMetadata {

      public String name,extension,owner,lastUser,lastModified,pathInOwner;
      public int id,size,lastVersion;
      public List<String> tags,users;


  }

Database
++++++++++

Para la persistencia de información (ej: no tener que registrarse cada vez que se abre la aplicación) se usa **Database** , un singleton que guarda y recupera datos básicos del usuario en forma de clave-valor.


TypedInputStream
++++++++++++++++++

Se encarga de transformar un archivo de la memoria interna a un tipo de datos compatible con retrofit para poder enviar al servidor. Además provee feedback de la cantidad de datos que fueron enviados para poder actualizar una barra de progreso a medida se suban los archivos.

ErrorDisplay
+++++++++++++++++

Singleton que muestra en pantalla mensajes de error o warnings.


RequestMaker
++++++++++++++++++

RequestMaker se encarga de transfromar la informacion capturada de la GUI a request http gracias a **ServiceGenerator** y a definir los callbacks que se ejecutan cuando se recibe la respuesta del servidor.

Está implementado con un patrón singleton y se accede desde varios puntos de la aplicación, abstrayendo y encapsulando todas las request que realiza el programa.





Código
------------------------
A continuación un ejemplo que muestra el flujo de información de la app.

* Acceso a una carpeta


Desde la ventana del drive se presiona una **FolderCard**. El evento es capturado por el listener::

 private void setCardsListeners(){
         .
         .
         .
         .
         this.recyclerFoldersView.addOnItemTouchListener(new RecyclerItemClickListener(getApplicationContext(), new RecyclerItemClickListener.OnItemClickListener() {


             @Override
             public void onItemClick(View view, int position) {
                 TextView clickedFolder = (TextView) view.findViewById(R.id.folder_name);
                 RequestMaker.getInstance().getUserFiles(activityCallback, email, token, path.goTo(clickedFolder.getText().toString()));
                 toggleUi(false);

             }


         }));


     }


Se realiza una llamada a **RequestMaker** para que realice la request al servidor

RequestMaker::

 public void getUserFiles(final NetworkCallbackClass activityCallback,String email,String token,String path){

         GetUserFilesService client = ServiceGenerator.createService(GetUserFilesService.class,baseUrl);

         client.getUserFiles(email, token, path, new Callback<GetUserFilesAnswer>() {
             @Override
             public void success(GetUserFilesAnswer getUserFilesAnswer, Response response) {
                 if (getUserFilesAnswer.result)
                     activityCallback.onGetUserFilesSuccess(getUserFilesAnswer);
                 else {
                     activityCallback.onRequestFailure(getUserFilesAnswer.errors);
                 }
             }

             @Override
             public void failure(RetrofitError error) {
                 activityCallback.onConnectionError();
             }
         });

     }

Una vez que se llama a client.getUserFiles(... ya se creara un nuevo thread. El que le sigue define los callbacks que se ejecutarán una vez que el server responda.

En este caso, la adaptación de la request la hace **GetUserFilesService**::

 public interface GetUserFilesService {
     @GET("/userfiles")
     void getUserFiles(@Query("email") String email,@Query("token") String token,@Query("path") String path,Callback<GetUserFilesAnswer> callback);
 }

Dentro del los callbacks de la request se llama a activityCallback, una instancia de **NetworkCallbackClass**, donde se realiza la conexión con la GUI::

 activityCallback.onGetUserFilesSuccess(getUserFilesAnswer);

**NetworkCallbackClass**::

 public void onGetUserFilesSuccess(GetUserFilesAnswer answer){
         callback.onGetUserFilesSuccess(answer);
     }

El método de la interfaz está implementado en **DriveActivity**

**GetUserFilesAnswer** es la clase a la cual se adaptan las respuestas de esta request::

   public class GetUserFilesAnswer {

       public static class Content {
           public List<Integer> files;C
           public List<String> folders;
       }
       public List<String> errors;
       public boolean result;
       public Content content;

   }


En caso de un error en la conexión, se llama a::

    activityCallback.onConnectionError();

Con su callback también implementado en **DriveActivity** donde se llama a **ErrorDisplay** para mostrar el error recibido::

 public void onConnectionError(){
         ErrorDisplay.getInstance().showMessage(context, view, "Connection error,check configured ip or try again later");
         toggleUi(true);
     }
