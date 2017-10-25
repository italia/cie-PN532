Documentazione
=================

    *«Una documentazione per domarli, una documentazione per trovarli,
    una documentazione per ghermirli e nell'oscurità incatenarli»*

La documentazione è stata creata, principalmente, mediante l'ausilio di un software chiamato Sphinx.
Quest'ultimo riesce a costrurire una vera e propria pagina ``html`` che è possibile visualizzare in un qualsisi
browser.
Il principale vantaggio è quella di ottenere una documentazione esteticamente gradevole, grazie a una vasta
scelta di temi disponibili e, sopratutto, chiara da comprendere.

Simbiosi con Breathe
---------------------------

Lo scopo finale di una documentazione è quello di comprendere in maniera approfondita quello che si sta per
andare a fare. Sphinx ci permette solamente di creare del puro e semplice testo, che spieghi quelli che sono 
i procedimenti da seguire per simulare, ed eseguire, il progetto.
La soluzione che abbiamo adotatto per implementare anche i commenti del nostro codice nella documentazione, e quindi 
fornire una documentazione ulteriormente efficace, è stata quella di implementare Breathe. Quest'ultima è una libreria 
che permette di creare un *bridge* tra Sphinx stesso e la formattazione dei commenti in Doxygen. In questo modo la documentazione
sarà strettamente correlata al codice, in quanto ad ogni nuovo update del codice la documentazione verrà aggiornata in maniera totalmente autonoma
e quindi permettere allo sviluppatore un vantaggio non indifferente.

Creazione della Documentazione
-------------------------------

Innanzitutto è fondamentale installare Breathe, che provvederà anche all'installazione di Sphinx
attraverso il seguente comando:

    ``pip install breathe``

Dopo aver corretamente installato il software richiesto bisognerà inizializzare Sphnix, quindi spostarsi 
nella ``directory`` in cui si vuole installare la *documentazione* stessa e lanciare il seguente comando:

    ``sphinx-quickstart``

Dopo aver fatto ciò, Sphinx chiederà all'utente di inserire una lista di settaggi totalmente personalizzabili
che andranno a generare poi quello che è il fulcro dell'intero progetto, ovvero un file **Make**. Quest'ultimo 
ogni volta che verrà eseguito genererà una pagina ``HTML`` in cui sarà possibile visualizzare l'intera documentazione.
Per generare quest'ultima bisognerà utilizzare il comando [1]_:

    ``make html``

L'intera documentazione quindi sarà modificabile semplicemente mediante la modfica e la creazione di file 
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