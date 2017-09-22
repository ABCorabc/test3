#include "StructFunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <assert.h>

int createList(List *lp)
{
	if (!lp) return 0;

	lp->head = (Node *)malloc(sizeof(Node));
	if (lp->head == NULL)
		return 0;

	lp->tail = (Node*)malloc(sizeof(Node));
	if (lp->tail == NULL) {
		free(lp->head);
		return 0;
	}

	lp->head->next = lp->tail;
	lp->tail->prev = lp->head;
	lp->head->prev = lp->head;
	lp->tail->next = lp->tail;

	return 1;
}

void destroyList(List *lp)
{
	Node *cur, *nextp;
	cur = lp->head->next;

	while (cur != lp->tail) {
		nextp = cur->next;
		free(cur);
		cur = nextp;
	}

	free(lp->head);
	free(lp->tail);
	lp->head = lp->tail = NULL;
}

int addLast(List *lp, void *data, size_t size, void(*memcpy)(void *, void*))
{
	if (lp == NULL)	return 0;
	Node *np;

	np = (Node*)malloc(sizeof(Node) + size);
	if (np == NULL)	return 0;

	memcpy(np + 1, data);
	np->next = lp->tail;
	lp->tail->prev->next = np;
	np->prev = lp->tail->prev;
	lp->tail->prev = np;

	return 1;
}

void displayList(List *lp, void(*dataPrint)(void *))
{
	if (lp == NULL) return;

	Node *cur;
	cur = lp->head->next;

	while (cur != lp->tail) {
		dataPrint(cur + 1);
		cur = cur->next;
	}
}

Node *searchNode(List *lp, void *data, int(*dataCompare)(void *, void *))
{
	if (lp == NULL) return NULL;
	Node *cur;
	cur = lp->head->next;

	while (cur != lp->tail) {
		if (dataCompare(cur + 1, data))
			return cur;
		cur = cur->next;
	}
}

int removeNode(List *lp, void *data, int(*dataCompare)(void *, void *))
{
	if (lp == NULL) return 0;
	Node *cur;
	cur = searchNode(lp, data, dataCompare);
	if (!cur) return 0;

	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	free(cur);
	return 1;
}

void sortList(List *lp, int(*compare1)(void *, void *), int(*compare2)(void *, void *), void(*memcpy)(void *, void *))
{
	Node *curp;
	Node *nextp;
	Student sData = { NULL,"",0,NULL };

	int num;

	if (lp == NULL) { /* lp포인터 NULL check */
		return;
	}

	printf("정렬 기준을 입력하시오(이름 : 1, 성적 : 2) :");
	scanf("%d", &num);

	curp = lp->head->next;

	while (curp->next != lp->tail) {
		nextp = curp->next;

		while (nextp != lp->tail) {

			switch (num) {
			case 1:
				if (compare1(curp + 1, nextp + 1) == 1) {
					memcpy(&sData, curp + 1);
					memcpy(curp + 1, nextp + 1);
					memcpy(nextp + 1, &sData);
				}
				break;
			case 2:
				if (compare2(curp + 1, nextp + 1) == 1) {
					memcpy(&sData, curp + 1);
					memcpy(curp + 1, nextp + 1);
					memcpy(nextp + 1, &sData);
				}
				break;
			}
			nextp = nextp->next;
		}
		curp = curp->next;
	}
}

void studentMemcpy(void *s1, void *s2)
{
	Student *p1 = (Student *)s1, *p2 = (Student *)s2;

	p1->name = (char*)malloc(sizeof(char) * (strlen(p2->name) + 1));
	strcpy(p1->name, p2->name);

	strcpy(p1->stdNum, p2->stdNum);
	p1->status = p2->status;
	scoreMemcpy(&p1->myScore, &p2->myScore);
}

void studentPrint(void *s)
{
	Student *cur = (Student *)s;
	printf("Name : %s\n", cur->name);
	printf("Student Number : %s\n", cur->stdNum);
	if (cur->status)
		printf("Student Status : %s\n", "재학");
	else
		printf("Student Status : %s\n", "휴학");
	scorePrint(&cur->myScore);
}

void scoreMemcpy(Score *s1, Score*s2)
{
	int i;

	s1->avgScore = s2->avgScore;

	if (s1->allClassName != NULL) {
		for (i = 0; i < s1->classCnt; i++)
			free(s1->allClassName[i]);
		free(s1->allClassName);
		free(s1->classScore);
	}

	s1->classCnt = s2->classCnt;

	s1->allClassName = (char**)malloc(sizeof(char*) * s2->classCnt);
	s1->classScore = (int *)malloc(sizeof(int) * s2->classCnt);

	for (i = 0; i < s1->classCnt; i++) {
		s1->allClassName[i] = (char*)malloc(sizeof(char) * (strlen(s2->allClassName[i]) + 1));
		strcpy(s1->allClassName[i], s2->allClassName[i]);
		s1->classScore[i] = s2->classScore[i];
	}
}

void scorePrint(Score *s)
{
	int i;

	printf("\n<< Score of each Class >>\n");

	for (i = 0; i < s->classCnt; i++)
		printf("%d. %s : %d 점\n", i + 1, s->allClassName[i], s->classScore[i]);
	printf("\nAverage Score : %.2lf\n", s->avgScore);
}

//void scholarship(List *lp, int(*compare)(void *, void *), void(*memcpy)(void *, void *))
//{
//	Node *curp;
//	Node *nextp;
//	Student sData = { NULL,"",0,NULL,NULL };
//
//	if (lp == NULL) { /* lp포인터 NULL check */
//		return;
//	}
//	
//	curp = lp->head->next;
//
//	while (curp->next != lp->tail) {
//		nextp = curp->next;
//
//		while (nextp != lp->tail) {
//			if (compare(curp + 1, nextp + 1) == -1) { // 점수가 더 낮은 경우
//				memcpy(&sData, curp + 1);
//				memcpy(curp + 1, nextp + 1);
//				memcpy(nextp + 1, &sData);
//			}
//			nextp = nextp->next;
//		}
//		curp = curp->next;
//	}
//
//	/*-------------------- 정렬 완료 -------------------*/
//	
//	
//}

//void scholarPrint(void *s)
//{
//	Student *cur = (Student *)s;
//	printf("----------장학금 이력 조회----------\n");
//	printf("Name : %s\n", cur->name);
//	printf("Student Number : %s\n", cur->stdNum);
//	if (cur->myScholar._nation == 1 && cur->myScholar._score == 1)
//		printf("국가장학금 성적장학금\n");
//	else if (cur->myScholar._nation == 1)
//		printf("국가장학금\n");
//	else if (cur->myScholar._score == 1)
//		printf("성적장학금\n");
//	else
//		printf("장학금 수혜 내역이 없습니다.\n");
//	printf("-----------------------------------\n\n");
//}

void scholarshipPrint(void *s)
{
	Scholarship *cur = (Scholarship *)s;
	printf("\n<< Scholarship >>\n");
	printf("Name : &s\n", cur->name);
	printf("Student Number : &s\n", cur->stdNum);
	for (int i = 0; i < cur->scholarshipCnt; i++) {
		printf("Name of Scholarship : %s\n", cur->scholarName);
		printf("Amount of Scholarship : %d\n", cur->sum);
		printf("Date : %d\n\n", cur->date);
	}
	return;
}

void scholarshipMemcpy(void *s1, void *s2)
{
	Scholarship *var1 = (Scholarship *)s1;
	Scholarship *var2 = (Scholarship *)s2;

	if (var2->name != NULL) {
		if (var1->name != NULL) {
			memset(var1->name, 0, strlen(var1->name));
			free(var1->name);
			var1->name = NULL;
		}
		var1->name = (char *)malloc(sizeof(char)*(strlen(var2->name) + 1));
		assert(var1->name != NULL);
	}
	else {
		if (var1->name != NULL) {
			memset(var1->name, 0, strlen(var1->name));
			free(var1->name);
			var1->name = NULL;
		}
	}
	strcpy(var1->stdNum, var2->stdNum);
	var1->scholarshipCnt = var2->scholarshipCnt;

	if (var1->scholarName != NULL) {
		free(var1->sum);
		for (int i = 0; i < var1->scholarshipCnt; i++) {
			free(var1->date[i]);
			free(var1->scholarName[i]);
		}
		//free(var1->date);
		free(var1->scholarName);
	}

	var1->sum = (int *)malloc(sizeof(int)*var2->scholarshipCnt);
	//var1->date = (int **)malloc(sizeof(int *)*var2->scholarshipCnt);
	for (int i = 0; i < 2; i++) {
		var1->date[i] = (int *)malloc(sizeof(int)*var1->scholarshipCnt);
		for (int j = 0; j < var2->scholarshipCnt; j++)
			var1->date[i][j] = var2->date[i][j];
	}

	var1->scholarName = (char **)malloc(sizeof(char *)*var2->scholarshipCnt);

	for (int i = 0; i < var2->scholarshipCnt; i++) {
		var1->sum[i] = var2->sum[i];
		//var1->date[i] = var2->date[i];
		strcpy(var1->scholarName[i], var2->scholarName[i]);
	}
}
