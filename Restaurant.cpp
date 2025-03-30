#include "main.h"

class imp_res : public Restaurant
{
public:
	customer* currentX = NULL;//vi tri X
	customer* currentheadline = NULL;//thu tu hien tai sau khi thay doi(lien ket doi)
	customer* currenttailline = NULL;
	customer* headqueue = NULL;//thu tu vao nha hang lien ket doi
	customer* tailqueue = NULL;
	int N = 0;
	
	
	int size_desk = 0;
	int size_line = 0;
	imp_res() {};
	void swapdata(customer* a, customer* b) {
		int tmp = a->energy;
		string s = a->name;
		a->energy = b->energy;
		a->name = b->name;
		b->energy = tmp;
		b->name = s;
	}
	void addPrev(customer* cus) {
		cus->next = currentX;
		cus->prev = currentX->prev;
		currentX->prev->next = cus;
		currentX->prev = cus;
	}
	void addNext(customer* cus) {
		cus->next = currentX->next;
		cus->prev = currentX;
		currentX->next->prev = cus;
		currentX->next = cus;
	}
	void addtotable(customer* cus)  {
		if (size_desk >= (MAXSIZE / 2)) {
			customer* tmp = currentX;
			customer* check = currentX;
			int RES = 0;
			do
			{
				if (RES < (abs(cus->energy - tmp->energy))) {
					RES = abs(cus->energy - tmp->energy);
					check = tmp;
				}
				tmp = tmp->next;
			} while (tmp != currentX);
			currentX = check;
			if ((cus->energy - check->energy) < 0) addPrev(cus);
			else addNext(cus);
		}
		else {
			if (currentX == NULL) {
				currentX = cus;
				currentX->next = cus;
				currentX->prev = cus;
			}
			else {
				if (currentX->next == currentX) {
					cus->next = currentX;
					cus->prev = currentX;
					currentX->next = cus;
					currentX->prev = cus;
				}
				else if (cus->energy >= currentX->energy) {
					addNext(cus);
				}
				else {
					addPrev(cus);
				}
			}
		}
		currentX = cus;
		size_desk++;
	}
	void removetable(customer* cus) {
		while (currentX->name != cus->name) {
			currentX = currentX->next;
		}
		customer* tmp = currentX;
		if (currentX->next != currentX) {
			currentX->next->prev = currentX->prev;
			currentX->prev->next = currentX->next;
			if (currentX->energy > 0) currentX = currentX->next;
			else currentX = currentX->prev;
		}
		else {
			currentX = NULL;
		}
		delete tmp;
		tmp = NULL;
		size_desk--;
	}
	void removeline(customer* cus) {
		customer* tmp = currentheadline;
		while (tmp)
		{
			if (tmp->name == cus->name) {
				if (tmp->next != NULL) tmp->next->prev = tmp->prev;
				if (tmp->prev != NULL) tmp->prev->next = tmp->next;
				if (tmp == currentheadline) currentheadline = currentheadline->next;
				if (tmp == currenttailline) currenttailline = currenttailline->prev;
				delete tmp;
				tmp = NULL;
				size_line--;
				break;
			}
			tmp = tmp->next;
		}
	}
	bool Notexistcurrent(customer* cus) {
		customer* tmp = headqueue;
		while (tmp)
		{
			if (tmp->name == cus->name) return false;
			tmp = tmp->next;
		}
		return true;
	}
	bool checkexisttable(customer* cus) {
		if (currentX == NULL) return false;
		else if (currentX->next == currentX) {
			if (currentX->name != cus->name) return false;
			return true;
		}
		else {
			customer* tmp = currentX;
			do
			{
				tmp = tmp->next;
				if (tmp->name == cus->name) return true;
			} while (tmp != currentX);
			return false;
		}
	}
	bool compare(customer* a, customer* b) {//a sau, b truoc
		if (abs(a->energy) > abs(b->energy)) return true;
		else if (abs(a->energy) < abs(b->energy)) return false;
		else if (abs(a->energy) == abs(b->energy)) {
			customer* tmp = headqueue;
			while (tmp)
			{
				if (tmp->name == a->name) return true;
				else if (tmp->name == b->name) return false;
				tmp = tmp->next;
			}
		}
	}
	customer* get(int idx,customer* head) {
		customer* tmp = head;
		for (int i = 0; i < idx; i++) {
			tmp = tmp->next;
		}
		return tmp;
	}
	void inssort2(customer* head, int n, int incr) {
		for (int i = incr; i < n; i += incr) {
			for (int j = i; j >= incr; j -= incr) {
				customer* a = get(j,head);
				customer* b = get(j-incr,head);
				if (compare(a, b)) {
					swapdata(a, b);
					N++;
				}
				else break;
			}
		}
	}
	void shellsort(customer* head, int n) {
		for (int i = n / 2; i > 2; i /= 2) {
			customer* start = head;
			for (int j = 0; j < i; j++) {
				inssort2(start, n - j, i);
				start = start->next;
			}
		}
		inssort2(head, n, 1);
	}
	void addqueue(customer* custom) {
		if (headqueue == NULL) {
			headqueue = custom;
			tailqueue = custom;
		}
		else {
			custom->prev = tailqueue;
			tailqueue->next = custom;
			tailqueue = custom;
		}
	}
	void RED(string name, int energy)
	{
		customer* cus = new customer(name, energy, nullptr, nullptr);
		if ((energy != 0)&&(Notexistcurrent(cus))) {
			customer* custom = new customer(name, energy, nullptr, nullptr);
			if (size_desk < MAXSIZE) {
				addtotable(cus);
				addqueue(custom);
			}
			else if (size_line < MAXSIZE) {
				if (currentheadline == NULL) {
					currentheadline = cus;
					currenttailline = cus;
				}
				else {
					cus->prev = currenttailline;
					currenttailline->next = cus;
					currenttailline = cus;
				}
				addqueue(custom);
				size_line++;
			}
		}
		else {
			delete cus;
		}
	}
	void BLUE(int num)
	{
		//cout << "blue "<< num << endl;
		if (num == 0) return;
		if (headqueue != NULL) {
			customer* tmp = headqueue;
			for (int i = 0; tmp && i < num;)
			{
				if (checkexisttable(tmp)) {
					customer* temp = tmp;
					if (tmp == headqueue) {
						headqueue = headqueue->next;
						tmp = tmp->next;
						if (headqueue != NULL) headqueue->prev = NULL;
						else tailqueue = NULL;
					}
					else {
						tmp->prev->next = tmp->next;
						if (tmp->next != NULL) tmp->next->prev = tmp->prev;
						tmp = tmp->next;
					}
					removetable(temp);
					delete temp;
					temp = NULL;
					i++;
				}
				else tmp = tmp->next;
			}
		}
		if (currentheadline != NULL) {
			for (int i = 0; currentheadline && size_desk < MAXSIZE; i++)
			{
				customer* tmp = currentheadline;
				currentheadline = currentheadline->next;
				if(currentheadline!=NULL) currentheadline->prev = NULL;
				tmp->next = NULL;
				addtotable(tmp);
				size_line--;
			}
		}
	}
	void PURPLE()
	{
		//cout << "purple"<< endl;
		if (currentheadline == NULL || currentheadline->next == NULL) return;
		else {
			customer* tmp = currentheadline;
			customer* saveMAX = currentheadline;
			int max = 0;
			int cnt = 0;
			while (tmp)
			{
				cnt++;
				if (abs(tmp->energy) >= abs(saveMAX->energy)) {
					saveMAX = tmp;
					max = cnt;
				}
				tmp = tmp->next;
			}
			shellsort(currentheadline, max);
			BLUE(N % MAXSIZE);
			N = 0;
		}
		//cout << N;
	}
	void REVERSAL()
	{
		//cout << "reversal" << endl;
		if (currentX == NULL || currentX->prev == currentX) return;
		else {
			string name = currentX->name;
			customer* tmp1 = currentX;
			customer* tmp2 = currentX->next;
			while (tmp1!=tmp2)
			{
				if (tmp1->energy > 0 && tmp2->energy > 0) {
					swapdata(tmp1, tmp2);
					if (tmp2->next == tmp1) break;
					tmp1 = tmp1->prev;
					tmp2 = tmp2->next;
				}
				else if (tmp1->energy > 0) tmp2 = tmp2->next;
				else if (tmp2->energy > 0) tmp1 = tmp1->prev;
				else {
					if (tmp2->next == tmp1) break;
					tmp1 = tmp1->prev;
					tmp2 = tmp2->next;
				}
			}
			tmp1 = currentX;
			tmp2 = currentX->next;
			while (tmp1 != tmp2)
			{
				if (tmp1->energy < 0 && tmp2->energy < 0) {
					swapdata(tmp1, tmp2);
					if (tmp2->next == tmp1) break;
					tmp1 = tmp1->prev;
					tmp2 = tmp2->next;
				}
				else if (tmp1->energy < 0) tmp2 = tmp2->next;
				else if (tmp2->energy < 0) tmp1 = tmp1->prev;
				else {
					if (tmp2->next == tmp1) break;
					tmp1 = tmp1->prev;
					tmp2 = tmp2->next;
				}
			}
			tmp1 = currentX;
			tmp2 = currentX;
			do
			{
				if (tmp1->name == name) {
					currentX = tmp1;
				}
				tmp1 = tmp1->next;
			} while (tmp1!=tmp2);
		}
	}
	void UNLIMITED_VOID()
	{
		//cout << "unlimited_void" << endl;
		if (size_desk >= 4) {
			int min1 = 0, min2=1000000, cnt=0,save=0;
			customer* tmp1 = currentX;
			customer* tail = currentX;
			customer* head1 = currentX;
			customer* head2 = currentX;
			customer* savemin1 = currentX;
			customer* savemin2 = currentX;
			do
			{
				customer* tmp2 = tmp1;
				head1 = tmp2;
				savemin1 = tmp2;
				do
				{
					cnt++;
					min1 += tmp2->energy;
					if (savemin1->energy > tmp2->energy) {
						savemin1 = tmp2;
					}
					if (cnt >= 4 && min2 > min1) {
						min2 = min1;
						save = cnt;
						tail = tmp2;
						head2 = head1;
						savemin2 = savemin1;

					}
					else if (cnt >= 4 && min2 == min1) {
						if (save <= cnt) {
							head2 = head1;
							tail = tmp2;
							save = cnt;
							savemin2 = savemin1;
						}
					}
					tmp2 = tmp2->next;
				} while (tmp2!=tmp1);
				cnt = 0;
				min1 = 0;
				tmp1 = tmp1->next;
			} while (tmp1!=currentX);
			customer* tmp = savemin2;
			do
			{
				tmp->print();
				if (tmp == tail) tmp = head2;
				else tmp = tmp->next;
			} while (tmp != savemin2);
		}
	}
	void DOMAIN_EXPANSION()
	{
		//cout << "domain_expansion" << endl;
		customer* tmp = headqueue;
		int sum1 = 0, sum2 = 0;
		while (tmp)
		{
			if (tmp->energy > 0) sum1 += tmp->energy;
			else sum2 += tmp->energy;
			tmp = tmp->next;
		}
		tmp = tailqueue;
		while (tmp)
		{
			if ((sum1 >= abs(sum2) && tmp->energy < 0) || (sum1 < abs(sum2) && tmp->energy > 0)) {
				tmp->print();
			}
			tmp = tmp->prev;
		}
		tmp = headqueue;
		while (tmp)
		{
			customer* temp = tmp;
			if ((sum1 >= abs(sum2) && tmp->energy < 0) || (sum1 < abs(sum2) && tmp->energy > 0)) {
				if (tmp->next != NULL) tmp->next->prev = tmp->prev;
				if (tmp->prev != NULL) tmp->prev->next = tmp->next;
				if (tmp == headqueue) headqueue = headqueue->next;
				if (tmp == tailqueue) tailqueue = tailqueue->prev;
				if (checkexisttable(tmp)) {
					removetable(tmp);
				}
				else {
					removeline(tmp);
				}
				tmp = tmp->next;
				delete temp;
				temp = NULL;
			}
			else tmp = tmp->next;
		}
		if (currentheadline != NULL) {
			for (int i = 0; currentheadline && size_desk < MAXSIZE; i++)
			{
				customer* temp = currentheadline;
				currentheadline = currentheadline->next;
				if (currentheadline != NULL) currentheadline->prev = NULL;
				temp->next = NULL;
				addtotable(temp);
				size_line--;
			}
		}
	}
	void LIGHT(int num)
	{
		//cout << "light " << num << endl;
		if (num == 0) {
			if (currentheadline != NULL) {
				customer* tmp = currentheadline;
				while (tmp) {
					tmp->print();
					tmp = tmp->next;
				}
			}
		}
		else {
			if (currentX != NULL) {
				customer* tmp = currentX;
				do
				{
					tmp->print();
					if (num > 0) tmp = tmp->next;
					else tmp = tmp->prev;
				} while (tmp != currentX);
			}
		}
	}
	~imp_res() {
		while (currentX) {
			removetable(currentX);//trong ham remove da co delete
		}
		while (currentheadline)
		{
			customer* tmp = currentheadline;
			currentheadline = currentheadline->next;
			delete tmp;
			tmp = NULL;
			size_line--;
		}
		while (headqueue)
		{
			customer* tmp = headqueue;
			headqueue = headqueue->next;
			delete tmp;
			tmp = NULL;
		}
	}
};