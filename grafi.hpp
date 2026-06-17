#pragma once
#include <list>
#include <map>
#include <string>
#include <set>
#include <unordered_map>
#include <iostream>
#include <cstdlib> //per std::abort()

template<typename T>
class unidirected_edge
{
    private:
        T primo;
        T secondo;
        bool invertito;
        std::string nome;
    public:
        unidirected_edge() = default;
        unidirected_edge(const T& a, const T& b,const std::string& n ="")
        {
            nome = n;
            if(a>b)
            {
                primo = b; secondo = a;
                invertito = true;
            }
            else
            {
                primo = a; secondo= b;
                invertito = false;
            }
        }

        T from() const {return primo;}
        T to() const {return secondo;}

        // per controllare se l'arco è stato invertito
        bool is_inverted() const{return invertito;}

        std::string get_name() const{return nome;}
        bool operator<(const unidirected_edge<T>& other) const
        {
            // sintassi con this e other, i.e se a<b: a lo ho gia ed è (primo,secondo), other sarebbe b
            // se i primi nodi sono diversi allora sara maggiore quello col primo nodo piu grande
            if(primo != other.primo)
            {
                return primo<other.primo;
            }
            // se i primi nodi sono uguali confronto il secondo to
            return secondo<other.secondo;
        }   
        bool operator==(const unidirected_edge<T>& other) const
        {
            // sintassi this e other
            if(primo == other.primo)
            {
                if(secondo == other.secondo) return true;
                else return false;
                
            }
            else return false;
        }   
};

// se mettessimo dentro la classe unidirected_edge prenderebbe come elemento di sinistra un tipo arco
template<typename T>
std::ostream&
operator<<(std::ostream& os, const unidirected_edge<T>& a)
{
    os << "(" << a.from() << "," << a.to() << ")";
    return os;
}


using namespace std;
                    //se non specifica il tipo nel main è in automatico double
template<typename T,typename P=double>
class unidirected_graph
{
    private:
        set<unidirected_edge<T>> archi; //per alledges(), set(archi)
        set<T> nodi;    //per allnodes(), set(nodi)
        map<T,set<T>> nadiacenti; //per neighours(), chiave è nodo
        map<unidirected_edge<T>,P> pesimap; //il valore associato a un arco è il peso 
        int n_resistenze = 0;
        public:
        unidirected_graph()=default;
        unidirected_graph(const unidirected_graph& other)
        {
            archi = other.archi;
            nodi = other.nodi;
            nadiacenti = other.nadiacenti;
            pesimap = other.pesimap;
            n_resistenze = other.n_resistenze;
        }

        set<T> neighours(const T& nodo) const
        {
            auto it = nadiacenti.find(nodo);
            //controllo se l'iteratore è diverso da end, in quel caso la chiave esiste
            if(it != nadiacenti.end())
                return it->second; //restituisco il valore trovato dall'iteratore
            else return set<T>();
        }
                     //passo sempre per riferimento ma come const cosi non posso modificare involontariamente
        void add_edge(const unidirected_edge<T>& newarc)
        {
            // insert su un set restituisce una coppia: iteratore e vero se l'ho inserito, 
            // falso se c'era già o altrimenti, per cui se l'ho inserito aggiungo il resto
            auto prova = archi.insert(newarc);
            if(prova.second)
            {
                nodi.insert(newarc.from());
                nodi.insert(newarc.to());
                nadiacenti[newarc.from()].insert(newarc.to());
                nadiacenti[newarc.to()].insert(newarc.from());
                pesimap[newarc] = 1;
                if(!newarc.get_name().empty() && newarc.get_name()[0] == 'R')
                {
                    n_resistenze++;
                }
            }
        }

        void rm_edge(const unidirected_edge<T>& oldarc)
        {
            //se l'arco non esiste nel set di archi non faccio nulla
            if(archi.find(oldarc) == archi.end()) 
                return;
            //altrmenti elimino l'arco dal set di archi, e elimino i nodi adiacenti e il peso
            auto u = oldarc.from();
            auto v = oldarc.to();
            archi.erase(oldarc);
            nadiacenti[u].erase(v);
            nadiacenti[v].erase(u);
            pesimap.erase(oldarc);
            if(!oldarc.get_name().empty() && oldarc.get_name()[0]=='R')
            {
                n_resistenze--;
            }
        }

        
        set<unidirected_edge<T>> all_edges() const {return archi;}
        set<T> all_nodes() const {return nodi;}

        int edge_number(const unidirected_edge<T>& arco) const 
        {
            //verifico se l'arco esiste 
            auto target = archi.find(arco);
        
            if (target == archi.end()) {
                return -1; // L'arco non c'è
            }

            //il set è ora ordinato, conto i passi dall'inizio fino al target
            int indice = 0;
            for (auto it = archi.begin(); it != target; ++it) {
                indice++;// Uso ++it (pre-incremento) per evitare copie inutili dell'iteratore
            }
            
            return indice;
        }

       // cambio int in size_t così che l'indice non può mai essere negativo e lo confronto con sizet
        unidirected_edge<T> edge_at(size_t num) const 
        {
            // confrontiamo size_t con size_t
            if (num >= archi.size()) 
            {
                std::cerr << "Errore: Indice dell'arco non valido" << std::endl;
                return unidirected_edge<T>(); 
            }

            auto it = archi.begin();
            
            // anche il contatore del for diventa size_t per coerenza
            for (size_t i = 0; i < num; i++) 
            {
                ++it; 
            }
            
            return *it; 
        }


        unidirected_graph<T,P> operator-(const unidirected_graph<T,P>& other) const
        {
            unidirected_graph<T,P> risultato;
            
            // scorriamo direttamente tutti gli archi di questo grafo
            for(const auto& corrente : archi)
            {
                // se l'arco 'corrente' NON è presente nell'altro grafo, lo aggiungo
                if(other.archi.find(corrente) == other.archi.end())
                {
                   risultato.add_edge(corrente);
                }
            }
            return risultato;
        }

        void add_peso(const unidirected_edge<T>& newarc,const P& peso)
        {
            if(archi.find(newarc) != archi.end())
            {
                pesimap[newarc] = peso;
            }
        }

        P get_peso(const unidirected_edge<T>& arc) const
        {
            if(archi.find(arc) != archi.end())
                return pesimap.at(arc);
            
            std::cerr << "Errore: stai cercando di leggere il peso di un arco inesistente "<<arc<<endl;
            std::abort(); //interrompo il programma se viene passato un arco inesistente
        }

        int get_resistenze() const {return n_resistenze;};

};


//creo un altro overload di << per stampare un grafo, ovvero i suoi archi uno dopo l'altro
template<typename T,typename P>
std::ostream&
operator<<(std::ostream& os, const unidirected_graph<T,P>& g)
{
    auto tutti_gli_archi = g.all_edges();
    size_t n = tutti_gli_archi.size();
    size_t i = 0; // uso size_t invece di int per fare confronti con n
    
    os << "{";
    for(const auto& arco : tutti_gli_archi)
    {
        os << arco;
        if(i != n - 1) os << ", "; // evito di stampare la virgola dopo l'ultimo arco
        i++;
    }
    os << "}\n";
    return os;
}


