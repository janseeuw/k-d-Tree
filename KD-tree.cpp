//
//  2DBoom.h
//  Algoritmen II
//
//  Created by Jonas Anseeuw on 6/01/14.
//  Copyright (c) 2014 Jonas Anseeuw. All rights reserved.
//

#ifndef Algoritmen_II__DBoom_h
#define Algoritmen_II__DBoom_h

#include <queue>
#include <deque>

using namespace std;

/* Interface */

template<class D>
class Boom2D;

template<class D>
class Knoop2D {
public:
	Knoop2D(D _punt, bool _x):punt(_punt),x(_x){};
	D punt;
	bool x;
	Boom2D<D> links, rechts;
};

template<class D>
class Boom2D {
public:
	Boom2D():k(0){};
	~Boom2D(){
		delete k;
	}
	Knoop2D<D> * k;
	void voegToe(D punt);
	int minimum(const bool x);
	void nearestNeighbor(const D &query, D &neighbor);
	void schrijf(std::ostream&) const;
private:
	int findminimum(const bool x, bool cd);
	void findNearestNeighbor(const D &query, D &currentBest, int &currentBestDistance);
	void schrijf(std::ostream& os,Knoop2D<D>* kn) const{
        if (kn!=0)
            os<<(kn->x? "x:":"y:")<<"("<<kn->punt.x<<","<<kn->punt.y<<")";
        else
            os<<"(,)";
    }
};

/* Implementatie */

template<class D>
void Boom2D<D>::voegToe(const D punt) {
	Boom2D<D>* plaats = this;
	bool x = true;
	while (plaats->k != 0) {
		int nieuwpunt = x ? punt.x : punt.y;
		int huidigpunt = x ? plaats->k->punt.x : plaats->k->punt.y;
		plaats = &((nieuwpunt < huidigpunt) ? plaats->k->links : plaats->k->rechts);
		x = !x;
	}
	if (plaats->k == 0) {
		plaats->k = new Knoop2D<D>(punt,x);
	}
}

template<class D>
int Boom2D<D>::minimum(const bool x){
	return findminimum(x, true);
}

template<class D>
int Boom2D<D>::findminimum(const bool x, bool cd){
	if(k==0) return INT_MAX;
	if(cd == x){
		if(k->links.k == 0){
			return x ? k->punt.x : k->punt.y;
		}else{
			return k->links.findminimum(x, !cd);
		}
	}else{
		int current = (x ? k->punt.x : k->punt.y);
		int smallestChild = min(k->links.findminimum(x, !cd), k->rechts.findminimum(x, !cd));
		return current < smallestChild ? current : smallestChild;
	}
}

template<class D>
void Boom2D<D>::nearestNeighbor(const D &query, D &neighbor){
	int currentBestDistance = INT_MAX;
	D currentBestPoint;
	findNearestNeighbor(query, currentBestPoint, currentBestDistance);
	neighbor=currentBestPoint;
}

template<class D>
void Boom2D<D>::findNearestNeighbor(const D &query, D &currentBestPoint, int &currentBestDistance){
	if(k != 0){
		// Search recursively to find the point in the same region as the query
		int queryVal = k->x ? query.x : query.y;
		int currentVal = k->x ? k->punt.x : k->punt.y;
		if(queryVal < currentVal){
			k->links.findNearestNeighbor(query,currentBestPoint,currentBestDistance);
		}else{
			k->rechts.findNearestNeighbor(query,currentBestPoint,currentBestDistance);
		}
		
		// On the return search each subtree where a closer point than the one you alreade know
		// about might be found
		
		if(currentBestDistance >= k->punt.distance(query)){
			currentBestDistance = k->punt.distance(query);
			currentBestPoint=k->punt;
		}
		
		Boom2D<D> *other;
		if(queryVal < currentVal){
			// Means we went to the left, check right tree
			other = &k->links;
		}else{
			// Means we went to the right, check left tree
			other =&k->rechts;
		}
		
		int distancePoints = k->x ? (query.x-k->punt.x)*(query.x-k->punt.x) :  (query.y-k->punt.y)*(query.y-k->punt.y);
		
		if(distancePoints < currentBestDistance){
			other->findNearestNeighbor(query, currentBestPoint, currentBestDistance);
		}
	}
}

template <class D>
void Boom2D<D>::schrijf(std::ostream& os) const{
    if (k!=0){
		std::queue<Knoop2D<D>*, std::deque<Knoop2D<D>*> > q;
		q.push(k);
		while(!q.empty()){
			Knoop2D<D>* nu=q.front();
			schrijf(os,nu);
			os<<" links: ";
			schrijf(os,nu->links.k);
			os<<" rechts: ";
			schrijf(os,nu->rechts.k);
			os<<std::endl;
			if (nu->links.k!=0)
				q.push(nu->links.k);
			if (nu->rechts.k!=0)
				q.push(nu->rechts.k);
			q.pop();
		}
    }
    else{
        schrijf(os,k);
    }
}

#endif
