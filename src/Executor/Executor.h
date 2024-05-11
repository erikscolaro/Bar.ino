
// cosa deve fare questa classe?
/*
1. ricevere una ricetta
2. ricevere la lista di ingredienti
    2.1 per ogni ingrediente nella lista di ingredienti:
        2.1.1 controllare se esiste nella lista di attuatori
        2.1.2 controllare se l'attuatore è connesso
            2.1.2.false -> abort + errore
            2.1.2.true -> continua al prossimo passo
3. usare l'iteratore della ricetta per ciclare sui singoli step
    2.1 controllo sull'azione con switch
        2.1.SKIP
        2.1.ADD -> ricevere l'indice dell'attuatore ed attuare
            2.1.ADD.1 ricordarsi si salvare in eeprom il valore della quantità aggionrato
        2.1.MIX -> eseguire
        2.1.SHAKE -> eseguire
4. ritornre al punto di partenza

necessari anche:
controlli sul tipo di bicchiere
controlli sul centramento del bicchiere
metodi per gestire il nastro trasportatore
funzioni di taratura per le valvole
metodo di ritorno all'inizio
qualcosa che dica a che punto siamo arrivati 
questa roba implementa anche l'interfaccia grafica!




*/