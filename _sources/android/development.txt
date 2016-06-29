..  _dev:


Project Development
===================

**Dr. Tinder** is an opensource project under **GPL3 licence**. Under the licence, is posible to generate derivated non-privative works. Also you can contribute to the project, all you need is a `Github <https://github.com/>`_ account and a `Git <https://es.wikipedia.org/wiki/Git>`_ client.


***********************
Development Environment
***********************

**Dr. Tinder** was developed entirely in *Android Studio*. We highly recomend *Android studio* as IDE for further develop. It can be downloaded from `here <https://developer.android.com/studio/index.html>`_.

Anyway, you could use any IDE with which you feel comfortable. **Dr. Tinder** has an automated build system thanks to `Gradle build tool <http://gradle.org/>`_ (Build script and gradle wrapper included).

************
Architecture
************

The app architecture is organized as follows:

.. image:: activityDiagram.png

The main activities are:

    * Login Activity: Acts as entry point to the system. Users can log in into the system or create a new user.
    * Main Activity: The main activity contains two fragments and is responsable for switching between them:

        * Selection Fragment: User selection screen
        * Chat list Fragment: List of all matches

        *Also, main activity contains a menu from which is accessible Profile Activity*

    * Profile Activity: Allows the user add or modify his data
    * User Details Activity: Shows the public user data of a candidate
    * Chat Session: Allows the user to chat with a contact.

There are three utility classes that provides access to the online resources. Every activity uses them to connect to the server via REST.

.. image:: server.png

* String Resources Handler
* Images Resources Handler
* User Data Handler

User Data Handler also provides a store place of the common user data (which is fetched from the server) to all the activities, such as the username or the session token.

***********
Data Policy
***********

**Dr. Tinder** stores no user data. Every information is fetched from the server dinamicaly and stored in RAM during the app execution. Even if data transfer rates are low, the data payload is small enought. If there is no network connection, the

The images are cached locally to improve network ussage in some cases, for example in the chats where if there where no cache, the same image would be fetch multiple times, slowing down the app. This cache is forced to clean up all chaced files in local storage when the main actictivity closes.

**************
Errors Display
**************

User errors are shown via snackbar. A snackbar wrapper is present in the "Utility" class. Errors that are irrelevant to the user such as some image failed to fetch are handled internally (and in a transparent way to user, like adding a "not found" image).

Every error is logged using the DrTinderLogger class, which is merely a wrapper of the log class. The class is used to add automatically an common tag to all the app logs (from our sourcecode).

****
Code
****

Here are some snippets that uses the most relevant features

* Filling an ImageView with an external resource using Image Resource Handler

.. code:: Java

    //UserDetailsActivity.java
    private void setUserImage(Intent intent) {

        ImageView imageView = (ImageView) findViewById(R.id.backdrop);
        String userId = intent.getStringExtra(EXTRA_USER_ID);

        ImageResourcesHandler.fillImageResource(userId, ImageResourcesHandler.RES_USER_IMG, mToken,
                imageView, getBaseContext());

    }

* Using Dr. Tinder logger to log an error

.. code:: Java

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK && requestCode == PICK_IMAGE) {
            Uri imageUri = data.getData();
            try {
                mProfileImage = MediaStore.Images.Media.getBitmap(this.getContentResolver(), imageUri);
            } catch (IOException e) {
                DrTinderLogger.writeLog(DrTinderLogger.ERRO, "The impossible happen. Image IOError");
                return; //No image to display (cant upload)
            }
            mProfilePic.setImageURI(imageUri);
        }
    }

* Making a query with String Resource Handler. Showing snackbar error

.. code:: Java

    StringResourcesHandler.executeQuery(mUsername, StringResourcesHandler.USER_INFO, mToken,
        data -> {
            if (data == null) {
                Utility.showMessage("Error de conexion con el servidor", Utility.getViewgroup(this), "Ok");
                return;
            }
            String username = data.get(0)[0];
            String age = data.get(0)[1];
            String sex = data.get(0)[2];
            String lookingFor = data.get(0)[3];
            String interest = data.get(0)[4];

            TextView usernameView = (TextView) findViewById(R.id.profUsername);
            assert usernameView != null; //DEBUG Assert
            usernameView.setText(username);

            mAge.setText(age);
            mSexMale.setChecked(sex.equals(MALE));
            mSexFemale.setChecked(sex.equals(FEMALE));
            mSearchingMale.setChecked(lookingFor.equals(MALE) || lookingFor.equals(BOTH));
            mSearchingFemale.setChecked(lookingFor.equals(FEMALE) || lookingFor.equals(BOTH));


            String[] interests = interest.split(StringResourcesHandler.INTEREST_DIVIDER);
            for (String interest1 : interests) {
                String[] params = interest1.split(StringResourcesHandler.INTEREST_DATA_DIVIDER);
                addInterest(params[0], params[1]);
            }

        });

* Clearing cached resource

.. code:: Java

    DrTinderLogger.writeLog(DrTinderLogger.INFO, "Rejected " + data.get(USER_NAME));
    ImageResourcesHandler.freeCachedResource(data.get(USER_ID),
            ImageResourcesHandler.RES_USER_IMG, getContext());

* Clearing all cache

.. code:: Java

    @Override
    public void onDestroy() {
        super.onDestroy();
        ImageResourcesHandler.clearCache(this);
    }

**********
Code Style
**********

**Dr. Tinder** follows a strict  code style guide to ensure code quality. Before making a pull request, please check the following rules:

    * Code builds without errors at Travis.
    * New code does not add errors at style checks performed by ``Checkstyle``, ``Findbugs``, ``PMD`` and ``Lint``.  You could check this locally using the *Gradle* ``check`` target as follows:

    .. code:: bash

         .\gradlew check

-----

************
Dependencies
************

**Dr. Tinder** depends on the following :ref:`external`

.. toctree::
   :maxdepth: 4

   libraries

-----

***********
Classes API
***********

The classes API can be found at :ref:`javadoc` section.

.. toctree::
   :maxdepth: 3

   ../packages
