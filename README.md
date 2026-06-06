# CircuitsProject

Problemi riscontrati

Prima Parte: lettura testo e inserimento nel grafo
Davide ha messo i pesi nella classe degli archi, mentre Simone e Francesco lo hanno messo nella
classe del grafo. Per quanto riguarda il nome dell'arco (V1 o R1..) e anche il booleano che controlla se l'arco è stato invertito oppure, lo abbiamo tutti messo nella classe degli archi

Abbiamo inoltre deciso per salvare il nome dell'arco di usare una stringa unica invece di char (V o R) + int (1,2,3...), usare la seconda era piu efficiente in termini di memoria, mentre abbiamo scelto la prima perche rendeva piu semplice la lettura e la scrittura del codice 

Abbiamo analizzato la costruzione dei cicli con la dfs e il coalbero, per implementarlo abbiamo cambiato il costruttore di default per la classe degli archi, in particolare per il nome associato a un arco, in questo modo possiamo usare la dfs direttamente come era gia stata implementata.

Per implementare i cicli con la dfs abbiamo deciso di usare la dfs non ricorsiva e usare lo stack esplicito e non quello implicito delle funzioni ricorsive. Abbiamo deciso di passare direttamente dal main l'albero, il coalbero e l'albero trovato con la dfs, e quindi cambiare gli input delle funzioni per trovare i cicli, cosi da passarlo una sola volta e non fare troppe operazioni

Per trovare i cicli minimi con de pina dobbiamo trovare i cicli minimi (bisogna capire a cosa si riferisce il termine minimi: se ai pesi o ai nodi) Nel contesto delle maglie di un circuito elettrico standard, "minimo" si riferisce al minor numero di archi (tutti gli archi hanno idealmente peso topologico 1). Cerchiamo la "finestrella" più stretta possibile. Se in futuro i rami avranno pesi diversi (es. impedenze), la BFS andrà sostituita con l'algoritmo di Dijkstra, ma l'architettura di De Pina rimarrà invariata.

Implementazione dello "Sdoppiamento" (Lifting Topologico): Per creare il grafo di supporto $G'$ (con i nodi sdoppiati $v^+$ e $v^-$) abbiamo scelto la via più semplice e meno invasiva: invece di creare una nuova classe Nodo, abbiamo semplicemente sommato $N$ (il numero totale dei nodi) all'indice originale ($v+N$). Questo ci ha permesso di riutilizzare la classe unidirected_graph esattamente com'era.
Abbiamo preso la decisione di design di far restituire alla BFS una semplice lista di nodi e di fare la conversione nel vettore binario di Eigen (0 e 1) solo alla fine, unicamente per il percorso vincente. Questo ha evitato che il programma perdesse tempo a costruire e manipolare enormi matrici per tentativi (percorsi) che sarebbero stati scartati.
Early-Exit nella Ricerca: Abbiamo implementato un'uscita anticipata (break) nella BFS non appena tocca il nodo di destinazione, evitando di esplorare inutilmente il resto della componente connessa.

abbiamo aggiunto get_nome nella classe di archi per vedere se abbiamo una resistenza o meno, per poi incrementare nella classe grafo  il numero di resistenza, aggiunto funzione get resistenze e cambianto remove edge


///implementare la soluzoine dei sistemi nel hpp, modificare eigen se serve, commentare e osservazioni

Nella costruzione delle matrici B, R e del vettore v per il calcolo delle correnti di maglia, abbiamo iterato su un vettore contenente le maglie, ottenuto come output dalla nostra implementazione dell'algoritmo di De Pina. In particolare abbiamo osservato che, siccome le correnti di maglia si trovano in un vettore risultato del prodotto scalare R'BR*v, se procediamo a riempire gli oggetti v, B e R nell'ordine delle maglie trovate da De Pina (che è difficile da prevedere) ci sarà corrispondenza tra le componenti e i prodotti scalari costruiranno le giuste equazioni. Inoltre, questa "imprevedibilità" dell'ordine delle maglie determina solamente l'ordine delle colonne di B, pertanto riempendo la righe di R e B seguendo l'ordine imposto dal grafo, otterremo un vettore vR la cui componente i-esima corrisponde al potenziale ai capi della resistenza i-esima seguendo la numerazione dell'input.