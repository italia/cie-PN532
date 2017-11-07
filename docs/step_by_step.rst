Documentazione
=================

    *«Una documentazione per domarli, una documentazione per trovarli,
    una documentazione per ghermirli e nell'oscurità incatenarli»*

Questa documentazione è stata generata mediante l’ausilio del software Sphinx, capace di generare pagine ``HTML``
responsive e con numerose possibilità di personalizzazione compatibili con qualsiasi browser. 
Di fatti si dimostra estremamente considerevole se si ha l'obbiettivo di ottenere una documentazione esteticamente 
gradevole, con numerosi temi già pronti per essere usati, e perfettamente leggibile.

Simbiosi con Breathe
---------------------------

Obbiettivo primario di una buona documentazione è raccogliere e catalogare tutte le informazioni relative 
all'argomento trattato. Presa questa definizione, la soluzione che abbiamo adottato per poter includere 
nella documentazione anche i commenti del nostro codice è stata quella di adoperare Breathe. 
Breathe è una libreria che permette di creare un *bridge* tra Sphinx e la formattazione dei commenti in Doxygen. 
In questo modo la documentazione sarà strettamente correlata al codice e ad ogni nuovo update del codice, 
la documentazione verrà aggiornata in maniera totalmente autonoma.

Creazione della Documentazione
-------------------------------

Innanzitutto è fondamentale installare Breathe, che provvederà anche all'installazione di Sphinx
attraverso il seguente comando:

    ``pip install breathe``

Dopo aver correttamente installato il software richiesto bisognerà inizializzare Sphinx, quindi spostarsi 
nella ``directory`` in cui si vuole installare la *documentazione* stessa e lanciare il seguente comando:

    ``sphinx-quickstart``

Dopo aver fatto ciò, Sphinx chiederà all'utente di inserire una lista di settaggi totalmente personalizzabili
che andranno a generare poi quello che è il fulcro dell'intero progetto, ovvero un file **Make**. Quest'ultimo 
ogni volta che verrà eseguito genererà una pagina ``HTML`` in cui sarà possibile visualizzare l'intera documentazione.
Per generare quest'ultima bisognerà utilizzare il comando [1]_:

    ``make html``

L'intera documentazione quindi sarà modificabile semplicemente mediante la modifica e la creazione di file 
con estensione **.rst** [2]_. 

Collegamenti
---------------

Di seguito è possibile reperire informazioni più dettagliate sui software utilizzati per la 
creazione della nostra documentazione:

    -   **Sphinx**: http://www.sphinx-doc.org/en/stable/ 
        
        - Tema utilizzato: https://github.com/italia/daf-docs/tree/master/_themes/sphinx_italia_theme
    
    -   **Breathe**: https://breathe.readthedocs.io/en/latest/index.html
    -   **Doxygen**: http://www.stack.nl/~dimitri/doxygen/


.. [1] Se sti lavorando su un repository Git è possibile adoperare il comando ``make clean`` che effettuerà una pulizia dei file generati precedentemente in modo tale da avere un commit più *pulito e snello* e quindi un'organizzazione migliore.
.. [2] E possibile reperire maggiori dettagli riguardo questo tipo di formattazione qui_.

.. _qui: http://docutils.sourceforge.net/docs/user/rst/quickref.html`_.