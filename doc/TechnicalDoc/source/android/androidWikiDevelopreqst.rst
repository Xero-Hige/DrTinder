..  _dev:


Project development
===================

You can contribute to **Dr. Tinder** project. **Dr. Tinder** is an opensource project under **GPL3 licence**. Under the licence, is posible to generate derivated non-privative works. Also you can contribute to the project, all you need is a `Github <https://github.com/>`_ account and a `Git <https://es.wikipedia.org/wiki/Git>`_ client.

We highly recomend *Android studio* as IDE to develop. It can be downloaded from `here <https://developer.android.com/studio/index.html>`_. Anyway, you could use any IDE with which you feel comfortable. **Dr. Tinder** has an automated build system thanks to `Gradle build tool <http://gradle.org/>`_ (Build script and gradle wrapper included).

**Dr. Tinder** follows a strict  code style guide to ensure code quality. Before making a pull request, please check the following rules:

    * Code builds without errors at Travis.
    * New code does not add errors at style checks performed by ``Checkstyle``, ``Findbugs``, ``PMD`` and ``Lint``.  You could check this locally using the *Gradle* ``check`` target as follows:
    .. code:: bash

         .\gradlew check
