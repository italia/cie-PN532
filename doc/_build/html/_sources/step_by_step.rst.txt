Documentazione
=================

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

Collegamenti
---------------

Di seguito è possibile reperire informazioni più dettagliate sui software utilizzati per la 
creazione della nostra documentazione:

    -   **Sphinx**: http://www.sphinx-doc.org/en/stable/ 
        
        - Tema utilizzato: https://github.com/italia/daf-docs/tree/master/_themes/sphinx_italia_theme
    
    -   **Breathe**: https://breathe.readthedocs.io/en/latest/index.html
    -   **Doxygen**: http://www.stack.nl/~dimitri/doxygen/