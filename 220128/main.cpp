#pragma warning(disable : 4996)
#pragma warning(disable : 6031)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

int main()
{
	//メモリストの構造体
	typedef struct _TXT
	{
		char memo[100];
		struct _TXT* next;
	}TXT;

	//一つのメモの中身に対する構造体
	typedef struct _MEMO
	{
		char txt;
		struct _MEMO* next;
	}MEMO;

	//メモの行数を数える為の構造体
	typedef struct _TOTAL
	{
		int retu;
		int gyou;
		struct _TOTAL* next;
		struct _TOTAL* before;
	}TOTAL;

	//メモのリストのポインタなど
	TXT* p = (TXT*)malloc(sizeof(TXT));
	TXT* lstart = p, * wp, * bef;

	//ひとつのメモに対するポンタなど
	MEMO* mp = (MEMO*)malloc(sizeof(MEMO));
	MEMO* mwp, * mstart = mp, * mbef = mp;
	mp->next = NULL;

	//行数などに対するポインタなど
	TOTAL* tp = (TOTAL*)malloc(sizeof(TOTAL));
	TOTAL* tstart = tp, * twp = tp, * tbef = tp, * trp, * tlast = tp;
	tp->next = NULL;
	tp->before = NULL;
	tp->gyou = 0;
	tp->retu = 0;

	//ファイルポインタ
	FILE* fp;

	char chr[100];
	char txt;
	char com[10];
	char name[100];

	//flgプログラム開始時にメモが既にあるかの判断など、２メモを開くときに存在するかどうか、ｍはメモ内に書き込みがあるか、ｔは二文字目からの処理の分岐、ｓはメモ内に書き込みがない状態
	int flg = 0, flg2 = 0, flgm = 0, flgt = 0, flgs = 1;
	int work = 0, workt = 0;

	//メモリストのファイルを開いて中身の有無を確認
	fp = fopen("memolist.txt", "r");
	while (fscanf(fp, "%s", chr) != EOF)
	{
		if (flg == 1)
		{
			p = (TXT*)malloc(sizeof(TXT));
			wp = lstart;
			while (wp->next != NULL)
			{
				wp = wp->next;
			}
			wp->next = p;
		}
		else flg = 1;
		strcpy(p->memo, chr);
		p->next = NULL;
	}
	fclose(fp);
	if (flg == 0)
	{
		printf("メモが存在しないためメモを作成してください。\n");
		printf("メモ名の最後には必ず.txtを入力してください\n");
		printf("メモ名->");
		scanf("%s", name);
		fp = fopen(name, "w");
		strcpy(p->memo, name);
		p->next = NULL;
		printf("\033[2J \033[1;1Hescを押すと入力作業が終了しファイルに保存されます。\n全角には対応されていません。\n");
		while (1)
		{
			if (kbhit())
			{
				txt = getch();
				if (txt == 0x1b)
				{
					mwp = mstart;
					while (mwp != NULL)
					{
						fputc(mwp->txt, fp);
						mwp = mwp->next;
					}
					break;
				}
				else
				{
					if (txt == '\r')
					{
						if (flgt == 0)flgt = 1;
						else mp = (MEMO*)malloc(sizeof(MEMO));
						
						mp->txt = txt;
						if (flgs == 1)
						{
							mstart = mp;
							mp->next = NULL;
							flgs = 0;
						}

						mwp = mstart;

						trp = tstart;
						while(trp != twp)
						{
							work += trp->retu;
							trp = trp->next;
						}
						work += workt;

						for (int i = 1; i < work; i++)
						{
							mwp = mwp->next;
						}
						if (mwp->next == NULL)mp->next = NULL;
						else mp->next = mwp->next;
						if (mwp == mstart && work == 0)
						{
							if(mwp->next != NULL)mp->next = mstart;
							mstart = mp;
							mwp = mp;
						}
						if (mwp != mstart || (mwp == mstart && work == 1))mwp->next = mp;

						tp = (TOTAL*)malloc(sizeof(TOTAL));
						tp->gyou = 0;
						tp->retu = 0;
						if (twp->next == NULL)tp->next = NULL;
						else
						{
							tp->next = twp->next;
							twp->next->before = tp;
						}
						twp->next = tp;
						tp->before = twp;

						if (twp->retu != workt)
						{
							tp->retu = twp->retu - workt;
							twp->retu = workt;
						}
						twp->retu += 1;
						twp = twp->next;
						for (int i=trp->gyou ;trp != NULL;i++)
						{
							trp->gyou = i;
							tlast = trp;
							trp = trp->next;
						}
						workt = 0;
						work = 0;
					}
					else if (txt == '\b')
					{
						if (flgs == 1)
						{
							continue;
						}
						mwp = mstart;

						trp = tstart;
						while (trp != twp)
						{
							work += trp->retu;
							trp = trp->next;
						}
						work += workt;

						mbef = mwp;
						for (int i = 1; i < work; i++)
						{
							mbef = mwp;
							mwp = mwp->next;
						}
						if (mbef != mwp)
						{
							mbef->next = mwp->next;
							twp->retu -= 1;
							workt -= 1;
						}
						else
						{
							if (mwp->next != NULL && workt != 0)
							{
								mstart = mwp->next;
								twp->retu -= 1;
								workt -= 1;
							}
							else if (workt != 0)
							{
								mstart = NULL;
								flgs = 1;
								workt = 0;
								twp->retu = 0;
							}
						}
						if (mwp->txt == '\r')
						{
							workt = trp->before->retu-1;
							trp->before->retu += trp->retu;
							trp->before->next = trp->next;
							if (trp->before->next == NULL && trp->before->before == NULL && workt == 0 && trp->before->retu <= 1)
							{
								trp->before->retu = 0;
								mstart = NULL;
								flgs = 1;
							}
							if (trp->next != NULL)	trp->next->before = trp->before;
							for (int i = trp->before->gyou; trp != NULL; i++)
							{
								trp->gyou = i;
								trp = trp->next;
							}
							twp = twp->before;
							tlast = twp;
						}
						work = 0;
					}
					else if (txt == 0xFFFFFFE0)
					{
						txt = getch();
						if (txt == 0x4b)//←
						{
							if (workt > 0)
							{
								workt -= 1;
							}
							else if(twp->gyou > 0)
							{
								workt = twp->before->retu - 1;
								twp = twp->before;
							}
						}
						else if (txt == 0x4d)//→
						{
							if (workt < twp->retu-1)
							{
								workt += 1;
							}
							else if (twp->gyou < tlast->gyou && twp != tlast)
							{
								twp = twp->next;
								workt = 0;
							}
							else if (workt < twp->retu)
							{
								workt += 1;
							}
						}
						else if (txt == 0x48)//↑
						{
							if (twp->gyou > 0)
							{
								twp = twp->before;
								if (workt > twp->retu-1)
								{
									workt = twp->retu - 1;
								}
							}
						}
						else if (txt == 0x50)//↓
						{
							if (twp->gyou < tlast->gyou)
							{
								twp = twp->next;
								if (workt > twp->retu-1 && twp != tlast)
								{
									if (twp->retu > 0)workt = twp->retu - 1;
									else workt = twp->retu;
								}
								else if(workt > twp->retu)
								{
									workt = twp->retu;
								}
							}
						}
					}
					else
					{
						if (flgt == 0)flgt = 1;
						else mp = (MEMO*)malloc(sizeof(MEMO));
						mp->txt = txt;
						if (flgs == 1)
						{
							mstart = mp;
							mp->next = NULL;
							flgs = 0;
						}

						mwp = mstart;

						trp = tstart;
						while (trp != twp)
						{
							work += trp->retu;
							trp = trp->next;
						}
						work += workt;

						for (int i = 1; i < work; i++)
						{
							mwp = mwp->next;
						}
						if (trp->gyou == 0 && trp->retu == 1 && mwp->txt=='\r')
						{
							mp->next=mwp;
							mstart = mp;
							mwp = mp;
						}
						else if (mwp->next == NULL)mp->next = NULL;
						else if (mwp == mstart && work == 0)
						{
							mp->next = mstart;
							mstart = mp;
							mwp = mp;
						}
						else mp->next = mwp->next;
						if (mwp != mstart ||(mwp == mstart && work == 1 && mstart != mp))mwp->next = mp;
						twp->retu += 1;
						workt += 1;
						work = 0;
					}
					flgm = 1;
				}
				printf("\033[2J \033[1;1Hescを押すと入力作業が終了しファイルに保存されます。\n全角には対応されていません。\n");
				mwp = mstart;
				while (mwp != NULL)
				{
					if (mwp->txt == '\r')printf("\n");
					else printf("%c", mwp->txt);
					mwp = mwp->next;
				}
				printf("\033[%d;%dH", twp->gyou+3, workt+1);
			}
		}
		fclose(fp);
	}
	printf("\033[2J \033[1;1H新規作成：new、メモ一覧を見る：list、開く：open、メモの削除、delete、終了：end\n");
	//バックスペース等の機能を追加
	while (1)
	{
		flg2 = 0;
		flg = 0;
		printf("操作->");
		scanf("%s", com);
		//新規作成処理
		if (strcmp(com, "new") == 0)
		{
			printf("メモ名の最後には必ず.txtを入力してください\n");
			printf("メモ名->");
			scanf("%s", name);
			wp = lstart;
			while (wp != NULL)
			{
				if (strcmp(wp->memo, name) == 0)
				{
					flg = 1;
					break;
				}
				wp = wp->next;
			}
			if (flg == 0)
			{
				p = (TXT*)malloc(sizeof(TXT));
				strcpy(p->memo, name);
				wp = lstart;
				if (lstart == NULL)	lstart = p;
				else
				{
					while (wp->next != NULL)
					{
						wp = wp->next;
					}
					wp->next = p;
				}
				p->next = NULL;
				mstart = NULL;
				work = 0;
				workt = 0;
				tp = (TOTAL*)malloc(sizeof(TOTAL));
				tstart = tp;
				twp = tstart;
				tp->gyou = 0;
				tp->retu = 0;
				tp->next = NULL;
				tp->before = NULL;
				flgs = 1;
				fp = fopen(name, "w");
				printf("\033[2J \033[1;1H%sを作成しました。\n", name);
				printf("\033[2J \033[1;1Hescを押すと入力作業が終了しファイルに保存されます。\n全角には対応されていません。\n");
				while (1)
				{
					if (kbhit())
					{
						txt = getch();
						if (txt == 0x1b)
						{
							mwp = mstart;
							while (mwp != NULL)
							{
								fputc(mwp->txt, fp);
								mwp = mwp->next;
							}
							break;
						}
						else
						{
							if (txt == '\r')
							{
								if (flgt == 0)flgt = 1;
								else mp = (MEMO*)malloc(sizeof(MEMO));

								mp->txt = txt;
								if (flgs == 1)
								{
									mstart = mp;
									mp->next = NULL;
									flgs = 0;
								}

								mwp = mstart;

								trp = tstart;
								while (trp != twp)
								{
									work += trp->retu;
									trp = trp->next;
								}
								work += workt;

								for (int i = 1; i < work; i++)
								{
									mwp = mwp->next;
								}
								if (mwp->next == NULL)mp->next = NULL;
								else mp->next = mwp->next;
								if (mwp == mstart && work == 0)
								{
									if (mwp->next != NULL)	mp->next = mstart;
									mstart = mp;
									mwp = mp;
								}
								if (mwp != mstart || (mwp == mstart && work == 1))mwp->next = mp;

								tp = (TOTAL*)malloc(sizeof(TOTAL));
								tp->gyou = 0;
								tp->retu = 0;
								if (twp->next == NULL)tp->next = NULL;
								else
								{
									tp->next = twp->next;
									twp->next->before = tp;
								}
								twp->next = tp;
								tp->before = twp;

								if (twp->retu != workt)
								{
									tp->retu = twp->retu - workt;
									twp->retu = workt;
								}
								twp->retu += 1;
								twp = twp->next;
								for (int i = trp->gyou; trp != NULL; i++)
								{
									trp->gyou = i;
									tlast = trp;
									trp = trp->next;
								}
								workt = 0;
								work = 0;
							}
							else if (txt == '\b')
							{
								if (flgs == 1)
								{
									continue;
								}
								mwp = mstart;

								trp = tstart;
								while (trp != twp)
								{
									work += trp->retu;
									trp = trp->next;
								}
								work += workt;

								mbef = mwp;
								for (int i = 1; i < work; i++)
								{
									mbef = mwp;
									mwp = mwp->next;
								}
								if (mbef != mwp)
								{
									mbef->next = mwp->next;
									twp->retu -= 1;
									workt -= 1;
								}
								else
								{
									if (mwp->next != NULL && workt != 0)
									{
										mstart = mwp->next;
										twp->retu -= 1;
										workt -= 1;
									}
									else if (workt != 0)
									{
										mstart = NULL;
										flgs = 1;
										workt = 0;
										twp->retu = 0;
									}
								}
								if (mwp->txt == '\r')
								{
									workt = trp->before->retu - 1;
									trp->before->retu += trp->retu;
									trp->before->next = trp->next;
									if (trp->before->next == NULL && trp->before->before == NULL && workt == 0 && trp->before->retu <= 1)
									{
										trp->before->retu = 0;
										mstart = NULL;
										flgs = 1;
									}
									if (trp->next != NULL)	trp->next->before = trp->before;
									for (int i = trp->before->gyou; trp != NULL; i++)
									{
										trp->gyou = i;
										trp = trp->next;
									}
									twp = twp->before;
									tlast = twp;
								}
								work = 0;
							}
							else if (txt == 0xFFFFFFE0)
							{
								txt = getch();
								if (txt == 0x4b)//←
								{
									if (workt > 0)
									{
										workt -= 1;
									}
									else if (twp->gyou > 0)
									{
										workt = twp->before->retu - 1;
										twp = twp->before;
									}
								}
								else if (txt == 0x4d)//→
								{
									if (workt < twp->retu - 1)
									{
										workt += 1;
									}
									else if (twp->gyou < tlast->gyou && twp != tlast)
									{
										twp = twp->next;
										workt = 0;
									}
									else if (workt < twp->retu)
									{
										workt += 1;
									}
								}
								else if (txt == 0x48)//↑
								{
									if (twp->gyou > 0)
									{
										twp = twp->before;
										if (workt > twp->retu - 1)
										{
											workt = twp->retu - 1;
										}
									}
								}
								else if (txt == 0x50)//↓
								{
									if (twp->gyou < tlast->gyou)
									{
										twp = twp->next;
										if (workt > twp->retu - 1 && twp != tlast)
										{
											if (twp->retu > 0)workt = twp->retu - 1;
											else workt = twp->retu;
										}
										else if (workt > twp->retu)
										{
											workt = twp->retu;
										}
									}
								}
							}
							else
							{
								if (flgt == 0)flgt = 1;
								else mp = (MEMO*)malloc(sizeof(MEMO));
								mp->txt = txt;
								if (flgs == 1)
								{
									mstart = mp;
									mp->next = NULL;
									flgs = 0;
								}

								mwp = mstart;

								trp = tstart;
								while (trp != twp)
								{
									work += trp->retu;
									trp = trp->next;
								}
								work += workt;

								for (int i = 1; i < work; i++)
								{
									mwp = mwp->next;
								}
								if (trp->gyou == 0 && trp->retu == 1 && mwp->txt == '\r')
								{
									mp->next = mwp;
									mstart = mp;
									mwp = mp;
								}
								else if (mwp->next == NULL)mp->next = NULL;
								else if (mwp == mstart && work == 0)
								{
									mp->next = mstart;
									mstart = mp;
									mwp = mp;
								}
								else mp->next = mwp->next;
								if (mwp != mstart || (mwp == mstart && work == 1 && mstart != mp))mwp->next = mp;
								twp->retu += 1;
								workt += 1;
								work = 0;
							}
							flgm = 1;
						}
						printf("\033[2J \033[1;1Hescを押すと入力作業が終了しファイルに保存されます。\n全角には対応されていません。\n");
						mwp = mstart;
						while (mwp != NULL)
						{
							if (mwp->txt == '\r')printf("\n");
							else printf("%c", mwp->txt);
							mwp = mwp->next;
						}
						printf("\033[%d;%dH", twp->gyou + 3, workt + 1);
					}
				}
				fclose(fp);
			}
			else
			{
				printf("%sは既に存在しています。\n", name);
			}
			printf("\033[2J \033[1;1H新規作成：new、メモ一覧を見る：list、開く：open、メモの削除、delete、終了：end\n");
		}
		//メモ一覧を見る処理
		else if (strcmp(com, "list") == 0)
		{
			printf("\033[2J \033[1;1H新規作成：new、メモ一覧を見る：list、開く：open、メモの削除、delete、終了：end\n");
			p = lstart;
			while (p != NULL)
			{
				printf("%s\n", p->memo);
				p = p->next;
			}
			if (lstart == NULL)
			{
				printf("メモが一つもありません。\n");
			}
		}
		//開く処理
		else if (strcmp(com, "open") == 0)
		{
			printf("どのメモを開きますか？->");
			scanf("%s", name);
			wp = lstart;
			while (wp != NULL)
			{
				if (strcmp(name, wp->memo) == 0)
				{
					flg2 = 1;
					break;
				}
				wp = wp->next;
			}
			if (flg2 == 0) printf("そのメモは存在していません。\n");
			else
			{
				mstart = NULL;
				work = 0;
				workt = 0;
				tp = (TOTAL*)malloc(sizeof(TOTAL));
				tstart = tp;
				twp = tstart;
				tp->gyou = 0;
				tp->retu = 0;
				tp->next = NULL;
				tp->before = NULL;
				flgs = 1;
				//ここから
				printf("\033[2J \033[1;1Hescを押すと入力作業が終了しファイルに保存されます。\n全角には対応されていません。\n");
				fp = fopen(name, "r");
				while (fscanf(fp, "%c", &txt) != EOF)
				{
					if (mstart == NULL)
					{
						mp = (MEMO*)malloc(sizeof(MEMO));
						mp->txt = txt;
						mstart = mp;
						mp->next = NULL;
						twp->retu += 1;
						workt += 1;
						if (txt == '\r')
						{
							twp = tstart;
							while (twp->next != NULL)
							{
								twp = twp->next;
							}
							tp = (TOTAL*)malloc(sizeof(TOTAL));
							tp->gyou = twp->gyou + 1;
							tp->retu = 0;
							twp->next = tp;
							tp->before = twp;
							tp->next = NULL;
						}
						flg = 1;
					}
					else
					{
						if (flg == 1)
						{
							mp = (MEMO*)malloc(sizeof(MEMO));
							mwp = mstart;
							while (mwp->next != NULL)
							{
								mwp = mwp->next;
							}
							mwp->next = mp;
						}
						else flg = 1;
						mp->txt = txt;
						mp->next = NULL;
						tp->retu += 1;
						workt += 1;
						if (txt == '\r')
						{
							twp = tstart;
							while (twp->next != NULL)
							{
								twp = twp->next;
							}
							tp = (TOTAL*)malloc(sizeof(TOTAL));
							tp->gyou = twp->gyou + 1;
							tp->retu = 0;
							twp->next = tp;
							tp->before = twp;
							tp->next = NULL;
							tlast = tp;
							workt = 0;
							twp = twp->next;
						}
					}
					flgm = 1;
					flgt = 1;
					flgs = 0;
				}
				mp = mstart;
				if (flgm == 1)
				{
					while (mp != NULL)
					{
						if (mp->txt == '\r')
						{
							printf("\n");
						}
						printf("%c", mp->txt);
						mp = mp->next;
					}
				}
				while (1)
				{
					if (kbhit())
					{
						txt = getch();
						if (txt == 0x1b)
						{
							mwp = mstart;
							while (mwp != NULL)
							{
								fputc(mwp->txt, fp);
								mwp = mwp->next;
							}
							break;
						}
						else
						{
							if (txt == '\r')
							{
								if (flgt == 0)flgt = 1;
								else mp = (MEMO*)malloc(sizeof(MEMO));

								mp->txt = txt;
								if (flgs == 1)
								{
									mstart = mp;
									mp->next = NULL;
									flgs = 0;
								}

								mwp = mstart;

								trp = tstart;
								while (trp != twp)
								{
									work += trp->retu;
									trp = trp->next;
								}
								work += workt;

								for (int i = 1; i < work; i++)
								{
									mwp = mwp->next;
								}
								if (mwp->next == NULL)mp->next = NULL;
								else mp->next = mwp->next;
								if (mwp == mstart && work == 0)
								{
									if (mwp->next != NULL)	mp->next = mstart;
									mstart = mp;
									mwp = mp;
								}
								if (mwp != mstart || (mwp == mstart && work == 1))mwp->next = mp;

								tp = (TOTAL*)malloc(sizeof(TOTAL));
								tp->gyou = 0;
								tp->retu = 0;
								if (twp->next == NULL)tp->next = NULL;
								else
								{
									tp->next = twp->next;
									twp->next->before = tp;
								}
								twp->next = tp;
								tp->before = twp;

								if (twp->retu != workt)
								{
									tp->retu = twp->retu - workt;
									twp->retu = workt;
								}
								twp->retu += 1;
								twp = twp->next;
								for (int i = trp->gyou; trp != NULL; i++)
								{
									trp->gyou = i;
									tlast = trp;
									trp = trp->next;
								}
								workt = 0;
								work = 0;
							}
							else if (txt == '\b')
							{
								if (flgs == 1)
								{
									continue;
								}
								mwp = mstart;

								trp = tstart;
								while (trp != twp)
								{
									work += trp->retu;
									trp = trp->next;
								}
								work += workt;

								mbef = mwp;
								for (int i = 1; i < work; i++)
								{
									mbef = mwp;
									mwp = mwp->next;
								}
								if (mbef != mwp)
								{
									mbef->next = mwp->next;
									twp->retu -= 1;
									workt -= 1;
								}
								else
								{
									if (mwp->next != NULL && workt != 0)
									{
										mstart = mwp->next;
										twp->retu -= 1;
										workt -= 1;
									}
									else if (workt != 0)
									{
										mstart = NULL;
										flgs = 1;
										workt = 0;
										twp->retu = 0;
									}
								}
								if (mwp->txt == '\r')
								{
									workt = trp->before->retu - 1;
									trp->before->retu += trp->retu;
									trp->before->next = trp->next;
									if (trp->before->next == NULL && trp->before->before == NULL && workt == 0 && trp->before->retu <= 1)
									{
										trp->before->retu = 0;
										mstart = NULL;
										flgs = 1;
									}
									if (trp->next != NULL)	trp->next->before = trp->before;
									for (int i = trp->before->gyou; trp != NULL; i++)
									{
										trp->gyou = i;
										trp = trp->next;
									}
									twp = twp->before;
									tlast = twp;
								}
								work = 0;
							}
							else if (txt == 0xFFFFFFE0)
							{
								txt = getch();
								if (txt == 0x4b)//←
								{
									if (workt > 0)
									{
										workt -= 1;
									}
									else if (twp->gyou > 0)
									{
										workt = twp->before->retu - 1;
										twp = twp->before;
									}
								}
								else if (txt == 0x4d)//→
								{
									if (workt < twp->retu - 1)
									{
										workt += 1;
									}
									else if (twp->gyou < tlast->gyou && twp != tlast)
									{
										twp = twp->next;
										workt = 0;
									}
									else if (workt < twp->retu)
									{
										workt += 1;
									}
								}
								else if (txt == 0x48)//↑
								{
									if (twp->gyou > 0)
									{
										twp = twp->before;
										if (workt > twp->retu - 1)
										{
											workt = twp->retu - 1;
										}
									}
								}
								else if (txt == 0x50)//↓
								{
									if (twp->gyou < tlast->gyou)
									{
										twp = twp->next;
										if (workt > twp->retu - 1 && twp != tlast)
										{
											if (twp->retu > 0)workt = twp->retu - 1;
											else workt = twp->retu;
										}
										else if (workt > twp->retu)
										{
											workt = twp->retu;
										}
									}
								}
							}
							else
							{
								if (flgt == 0)flgt = 1;
								else mp = (MEMO*)malloc(sizeof(MEMO));
								mp->txt = txt;
								if (flgs == 1)
								{
									mstart = mp;
									mp->next = NULL;
									flgs = 0;
								}

								mwp = mstart;

								trp = tstart;
								while (trp != twp)
								{
									work += trp->retu;
									trp = trp->next;
								}
								work += workt;

								for (int i = 1; i < work; i++)
								{
									mwp = mwp->next;
								}
								if (trp->gyou == 0 && trp->retu == 1 && mwp->txt == '\r')
								{
									mp->next = mwp;
									mstart = mp;
									mwp = mp;
								}
								else if (mwp->next == NULL)mp->next = NULL;
								else if (mwp == mstart && work == 0)
								{
									mp->next = mstart;
									mstart = mp;
									mwp = mp;
								}
								else mp->next = mwp->next;
								if (mwp != mstart || (mwp == mstart && work == 1 && mstart != mp))mwp->next = mp;
								twp->retu += 1;
								workt += 1;
								work = 0;
							}
							flgm = 1;
						}
						printf("\033[2J \033[1;1Hescを押すと入力作業が終了しファイルに保存されます。\n全角には対応されていません。\n");
						mwp = mstart;
						while (mwp != NULL)
						{
							if (mwp->txt == '\r')printf("\n");
							else printf("%c", mwp->txt);
							mwp = mwp->next;
						}
						printf("\033[%d;%dH", twp->gyou + 3, workt + 1);
					}
				}
				fclose(fp);
				printf("\033[2J \033[1;1H新規作成：new、メモ一覧を見る：list、開く：open、メモの削除、delete、終了：end\n");
			}
		}
		//削除処理
		else if (strcmp(com, "delete") == 0)
		{
			printf("どのメモを削除しますか？->");
			scanf("%s", name);
			p = lstart;
			bef = NULL;
			while (p != NULL)
			{
				if (strcmp(name, p->memo) == 0)
				{
					flg2 = 1;
					break;
				}
				bef = p;
				p = p->next;
			}
			printf("\033[2J \033[1;1H新規作成：new、メモ一覧を見る：list、開く：open、メモの削除、delete、終了：end\n");
			if (flg2 == 0) printf("%sが見つかりませんでした。\n", name);
			else
			{
				if (p == lstart)
				{
					remove(name);
					lstart = p->next;
				}
				else
				{
					remove(name);
					bef->next = p->next;
				}
				printf("ファイルを削除しました。\n");
			}
		}
		//終了処理
		else if (strcmp(com, "end") == 0)
		{
			fp = fopen("memolist.txt", "w");
			p = lstart;
			while (p != NULL)
			{
				fputs(p->memo, fp);
				fputc('\n', fp);
				p = p->next;
			}
			fclose(fp);
			return 0;
		}
		else printf("コマンドが違います\n");
	}
}