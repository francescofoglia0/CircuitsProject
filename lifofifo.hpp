#pragma once
#include <stack>
#include <queue>

template <typename T>
class lifo
{   
    private:
        std::stack<T> s;
    
    public:
        lifo() = default;
        
        void put(const T& n)
        {
            s.push(n); //inserisco elemento n alla fine di stack
        }

        T get()
        {
            T val = s.top(); //estraggo primo elemento stack
            s.pop(); //rimuovo il top element
            return val;
        }

        bool empty() const
        {
            return s.empty();
        }
};

template<typename T>
class fifo
{
    private:
        std::queue<T> q;
    public:
        fifo() = default;

        void put(const T& n)
        {
            q.push(n); //aggiunge in fondo alla coda
        }

        T get()
        {
            T val = q.front(); //prendo il primo elemento arrivato
            q.pop(); //lo elimino
            return val;
        }
        bool empty() const
        {
            return q.empty();
        }
};


//creo anche una struct per gli elementi da passare alla priortiy queue che ridefinisco
template<typename T,typename P>
struct elementoPQ
{
    T nodo;
    P peso;
    //per questi elementi mi serve che nella priority queue l'ordinamento sia crescente (il top
    //elemento in coda sia il piu piccolo), quindi ridefinisco l'operatore di confronto, poiche di
    //base la priority queue li "ordina" al contrario (il piu grande è in top)
    bool operator<(const elementoPQ& other) const
    {
        return peso>other.peso;
    }
};

template <typename T, typename P>
class priorityQ {
private:
    std::priority_queue<elementoPQ<T, P>> pq;
public:
    priorityQ() = default;

    void put(const T& nodo, const P& peso) 
    { 
        pq.push({nodo, peso}); 
    }

    elementoPQ<T, P> get() 
    {
        auto t = pq.top();
        pq.pop();
        return t;
    }

    bool empty() const 
    { 
        return pq.empty();
    }
};