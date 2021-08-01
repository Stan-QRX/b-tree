typedef  struct btree1
{
	int sumkey;       //关键字总数   每个节点的关键字总数都  不一样
	struct btree1* kid[m+1];   //   孩子指针数组从  0  开始 ！  多余的一个指针数组元素存放分裂的结点地址 本程序没放（因为没用）
	datatype key[m];   //多一位   key[m-1]空出来    关键字数组从  0 至 m-2 ！
	struct btree1* parent;
}btree;
void bianli(btree* shu)
{
	if (shu == 0)
		return;
	for (int i = 0; i < shu->sumkey; i++)
	{
		cout << shu->key[i] << "  ";
	}
	for (int i = 0; i <= shu->sumkey; i++)
		bianli(shu->kid[i]);
}
int getmid(int x)
{
	
	if (x % 2 == 0)
		x = x / 2;
	else
		x= x / 2 + 1;
	return x;
}
int  zhao(btree *shu,int x, btree** xjie)//key[m-1]空出来 查找成功返回1, *xjie指向该结点；失败返回0, *xjie指向查找失败的结点
{
	int tag = 0;
	if (shu == 0)
	{
		return 0;
	}
	
	for (int i = 0; i < shu->sumkey; i++)
	{
		if (x == shu->key[i])
		{
			(*xjie) = shu;
			tag = 1;
			break;
		}
		else
			if (x < shu->key[i])
			{
				(*xjie) = shu;
				tag = zhao(shu->kid[i ],x,xjie);
				break;
			}
			else
				if (i == shu->sumkey-1)   //只可能是 大于  情况 且 在 kid[i+1]中寻找
				{
					(*xjie) = shu;
					tag = zhao(shu->kid[i+1], x, xjie);
					break;
				}
	}
	return tag;
}
int chashuzu(int a[], int sum,int x)  //  将 x 插入到 a数组中合适的位置，使之依然有序，由小及大  a[m-1]空出来
{
	int n=sum;
	
		for (int i = 0; i < sum; i++)
		{
			if (a[i] > x)
			{
				n = i;
				break;
			}
		}
		if (n == sum )
		{
			a[n] = x;
		}
		else
		{
			for (int i = sum-1; i != n - 1;i--)
			{
				a[i + 1] = a[i];
			}
			a[n] = x;
		}
		
		return 1;

}
int fenlie(btree** shu, btree* p, int x)  // shu为头结点指针，p指向当前应被分裂结点，x为插入元素
{
	int kidsum = p->sumkey + 1;  //  从 0到 kidsum  （p->kid[]）
	chashuzu(p->key, p->sumkey, x);
	int mid = getmid(p->sumkey );  // mid从 0 开始   p->sumkey为最大关键字数
	int temp = p->key[mid];
	for (int i = mid + 1; i != p->sumkey + 1; i++)   //ok
	{
		p->key[i - 1] = p->key[i];
	}
	
	if (p == *shu)   //               树 结点指针需要分裂
	{
		btree* xin1 = (btree*)malloc(sizeof(btree));
		btree* xin2 = (btree*)malloc(sizeof(btree));
		xin1->key[0] = temp;
		xin1->sumkey = 1;
		xin1->parent = 0;
		xin1->kid[0] = *shu;
		xin1->kid[1] = xin2;

		xin2->sumkey = p->sumkey - mid;
		int q = 0;
		for (int i = mid; i < p->sumkey; i++)
		{
			xin2->key[q] = p->key[i];
			q++;
		}
		xin2->parent = xin1;
		if (p->kid[0] != 0)  //  (非叶结点)  让新分裂结点指向自己的孩子
		{
			int e = 0;
			for (int i = mid + 1; i <= kidsum; i++)  //  kid[]
			{
				xin2->kid[e] = p->kid[i];

				e++;
			}
			for (int i = 0; i < e; i++)
			{
				xin2->kid[i]->parent = xin2;
			}
		}
		else                   //叶结点
		{
			for (int i = 0; i <=m ; i++)
				xin2->kid[i] = 0;

		}

		p->parent = xin1;
		p->sumkey = mid;

		*shu = xin1;
	}
	else                         // 非根节点分裂
	{
		int r = 0;
		while (p != p->parent->kid[r])  //记录将被分裂的结点在父母结点的孩子指针数组的下标==r
		{
			r++;
		}
		btree* xin = (btree*)malloc(sizeof(btree));   // xin 为新分裂的结点指针

		int j = 0;
		for (int i = mid; i < p->sumkey; i++)
		{
			xin->key[j] = p->key[i];
			j++;
		}
		//p->parent ->kid[m] = xin;           //  kid[m]  存放分裂的结点地址
		xin->parent = p->parent;

		for (int i = p->parent->sumkey + 1; i >= r + 2; i--)// 对该节点的父母结点而言
		{
			p->parent->kid[i] = p->parent->kid[i - 1];
		}
		p->parent->kid[r + 1] = xin;   // kid[r+1]  存放分裂的结点地址


		if (p->kid[0] != 0)  //  (非叶结点)  让新分裂结点指向自己的孩子
		{
			int e = 0;
			for (int i = mid + 1; i <= kidsum; i++)
			{
				xin->kid[e] = p->kid[i];

				e++;
			}
			for (int i = 0; i < e; i++)
			{
				xin->kid[i]->parent = xin;
			}
		}
		else                   //叶结点
		{
			for (int i = 0; i <= m; i++)
				xin->kid[i] = 0;

		}

		p->sumkey = mid;
		xin->sumkey = j;


		if (p->parent->sumkey < m - 1)  //父母结点没满
		{
			chashuzu(p->parent->key, p->parent->sumkey, temp);// 给父母结点插入元素    注意sumkey!!!
			p->parent->sumkey++;

		}
		else                              // 父母结点已满,父母结点需要分裂
		{
			fenlie(shu, p->parent, temp);

		}
	}
	return 1;
}
int cha(btree** shu, int x)            //  notice keysum,父母结点!!! 
{
	btree* p = 0;
	
	if (zhao(*shu, x, &p) == 1)
	{
		cout << "插入失败，树中已有该元素" << endl;
		return 0;
	}
	if (p == 0)
	{
		p = *shu;
	}
	if (p->sumkey < m - 1)
	{
		chashuzu(p->key, p->sumkey, x);
		p->sumkey++;
	}
	else
	{
		fenlie(shu, p, x);
	}
	return 1;

}
int minshu(btree* shu,int *a, btree** p)  // 成功返回1，x==所指的最小子树的最小关键字,shu==根节点，p为其地址
{
	if (shu == 0)
		return 0;
	*p = shu;
	*a = shu->key[0];
	while ((*p)->kid[0] != 0)
	{
		*p = (*p)->kid[0];
		*a = (*p)->key[0];
	}
	return 1;
}
int hebingfeiye(int r,btree* p,btree* bro)   //  合并结点 （     非叶结点！！！！    ） 
//                                               删除函数 情况3 要用到   多了 考虑kid情况
//                                               将父母结点的一个关键字及兄弟结点的全部关键字赋值于自身结点（p）
//本函数虽然未考虑🤔 当 p的父母为根节点时，删除父母结点的空间情况，但本函数目前只被shan函数调用，删函数中考虑了此情况，
// 见情况3 while循环 第一个if语句;因此在其他函数调用本函数时应考虑此情况。
{
	btree* par = p->parent;
	int sum = p->sumkey;
	if (r == 0)  //  右兄弟
	{
		p->key[p->sumkey] = par->key[0];
		p->sumkey++;
		int count = p->sumkey;
		for (int i = 0; i < bro->sumkey; i++)
		{
			p->key[count] = bro->key[i];
			count++;
		}
		p->sumkey = p->sumkey + bro->sumkey;




		for (int i = 0; i <= bro->sumkey; i++)  //  这部分为 非叶结点 考虑的情况
		{
			if(bro->kid [i]!=0)  // 后面父母结点为根节点情况要调用该函数，可能出现 ==0 情况
			bro->kid[i]->parent = p;
		}
		int b = 0;
		for (int i = sum + 1; i <= p->sumkey; i++)
		{
			p->kid[i] = bro->kid[b];
			b++;
		}

		free(par->kid[1]);                 //par    非叶结点注意最后删     kid


		par->kid[1] = 0;   //  防止 par->kid[1] 是最右边指针，不会有指针代替

		for (int i = 2; i < par->sumkey + 1; i++)
		{
			par->kid[i - 1] = par->kid[i];
		}
		for (int i = 1; i < par->sumkey; i++)
		{
			par->key[i - 1] = par->key[i];
		}
		par->sumkey--;

	}
	else   // 左兄弟
	{
		chashuzu(p->key, p->sumkey, par->key[r - 1]);
		p->sumkey++;
		int count = p->sumkey;
		for (int i = 0; i < bro->sumkey; i++)
		{
			chashuzu(p->key, count, bro->key[i]);
			count++;
		}
		p->sumkey = p->sumkey + bro->sumkey;


		for (int i = 0; i <= bro->sumkey; i++)  //  这部分为 非叶结点 考虑的情况
		{
			if (bro->kid[i] != 0)  // 后面父母结点为根节点情况要调用该函数，可能出现 ==0 情况
			bro->kid[i]->parent = p;
		}
		int c = p->sumkey;
		for (int i = sum; i >= 0; i--)
		{
			p->kid[c] = p->kid[i];
			c--;
		}
		for (int i = 0; i <= bro->sumkey; i++)
		{
			p->kid[i] = bro->kid[i];
		}
		
		

		free(par->kid[r - 1]);                 //par
		par->kid[r-1] = 0;
		for (int i = r; i < par->sumkey + 1; i++)
		{
			par->kid[i - 1] = par->kid[i];
		}
		for (int i = r; i < par->sumkey; i++)
		{
			par->key[i - 1] = par->key[i];
		}
		par->sumkey--;
	}
	return 1;
}
int shan(btree** shu, int x) // 根节点指针在该函数中并不会改变
{
	btree* bro=0;
	btree* p = 0;
	int temp;

	if (zhao(*shu, x, &p) == 0)
	{
		cout << "删除失败，树中没有有该元素" << endl;
		return 0;
	}
	int n=0;      // 存储被删除数在结点的数组下标
	for (int i = 0; i < p->sumkey; i++)
	{
		if (p->key[i] == x)
		{
			n = i;
			break;
		}

	}
	int r=0;       // 存储被删除元素的结点在父母结点的 孩子指针数组的下标
	if (p->parent != 0)
	{
		for (int i = 0; i <= p->parent->sumkey; i++)
		{
			if (p->parent->kid[i] == p)
			{
				r = i;
				break;
			}
		}
		if (r == 0)
		{
			bro = p->parent->kid[1];

		}
		else
		{
			bro = p->parent->kid[r - 1];

		}
	}
	if (p->kid[0] == 0)  //叶结点
	{
		if (p->sumkey >= getmid(m))  //   情况1
		{

			for (int i = n + 1; i < p->sumkey; i++)
			{
				p->key[i - 1] = p->key[i];
			}
			p->sumkey--;
		}
		else
			if (p->parent == 0) // 情况太复杂，不与考虑
			{
				cout << "删除的为根节点元素，且若删除该元素，将不满足b树的定义,因此不执行任何操作，请重新选择删除元素" << endl;
				return 0;
			}
			else
				if (bro->sumkey >= getmid(m))            //    情况2
				{
					if (r == 0)
					{
						temp = p->parent->key[0];
						p->parent->key[0] = bro->key[0];
						for (int i = 1; i < bro->sumkey; i++)
						{
							bro->key[i - 1] = bro->key[i];

						}
						bro->sumkey--;
						for (int i = n + 1; i < p->sumkey; i++)
						{
							p->key[i - 1] = p->key[i];
						}
						chashuzu(p->key, p->sumkey - 1, temp);
					}
					else
					{
						temp = p->parent->key[r - 1];
						p->parent->key[r - 1] = bro->key[bro->sumkey - 1];

						bro->sumkey--;
						for (int i = n + 1; i < p->sumkey; i++)
						{
							p->key[i - 1] = p->key[i];
						}
						chashuzu(p->key, p->sumkey - 1, temp);
					}
				}
				else             //    情况3
				{
					btree* par = p->parent;
					for (int i = n + 1; i < p->sumkey; i++)   //删除元素
					{
						p->key[i - 1] = p->key[i];
					}
					p->sumkey--;

					if (par->sumkey >= getmid(m))   //  父母结点有足够的关键字
					{
						//  以下 为合并结点  (  叶结点  ）
						if (r == 0)  //  右兄弟
						{
							if(p->sumkey>m-2 )
							{ 
								cout << "删除失败，程序出错，越界！" << endl;
								return 0;
							}
							p->key[p->sumkey] = par->key[0];  //上面加的if语句已经排除了警告（越界）
							p->sumkey++;
							int count = p->sumkey;
							for (int i = 0; i < bro->sumkey; i++)
							{
								p->key[count] = bro->key[i];
								count++;
							}
							p->sumkey = p->sumkey + bro->sumkey;



							free(par->kid[1]);                 //par    非叶结点注意最后删     kid
							par->kid[1] = 0;
							for (int i = 2; i < par->sumkey + 1; i++)
							{
								par->kid[i - 1] = par->kid[i];
							}
							for (int i = 1; i < par->sumkey; i++)
							{
								par->key[i - 1] = par->key[i];
							}
							par->sumkey--;

						}
						else   // 左兄弟
						{
							chashuzu(p->key, p->sumkey, par->key[r - 1]);
							p->sumkey++;
							int count = p->sumkey;
							for (int i = 0; i < bro->sumkey; i++)
							{
								chashuzu(p->key, count, bro->key[i]);
								count++;
							}
							p->sumkey = p->sumkey + bro->sumkey;


							free(par->kid[r - 1]);                 //par
							par->kid[r - 1] = 0;
							for (int i = r; i < par->sumkey + 1; i++)
							{
								par->kid[i - 1] = par->kid[i];
							}
							for (int i = r; i < par->sumkey; i++)
							{
								par->key[i - 1] = par->key[i];
							}
							par->sumkey--;
						}
					}
					else      // 父母结点 关键字不够
					{
						hebingfeiye(r, p, bro);          //  先对原先的 进行合并结点

						btree* newp = p;
						btree* newpar = p->parent;
						btree* newbro = bro;
						while (newpar->sumkey <= getmid(m) - 1)  // 第一次while循环一定可以进去
						{
							if (newpar == *shu)  //  父母结点为 根节点
							{
								if (newpar->sumkey == 0)
								{
									*shu = newp;
									newp->parent = 0;

									free(newpar);
									newpar = 0;
								}
								break;
							}
							else
							{
								newp = p->parent;
								newpar = newp->parent;

								int r1 = 0;      // 存储被删除元素的结点在父母结点的 孩子指针数组的下标，共同部分,在while循环中的
								for (int i = 0; i <= newp->parent->sumkey; i++)
								{
									if (newp->parent->kid[i] == newp)
									{
										r1 = i;
										break;
									}
								}
								if (r1 == 0)
								{
									newbro = newp->parent->kid[1];
								}
								else
								{
									newbro = newp->parent->kid[r1 - 1];
								}


								if (newbro->sumkey == getmid(m) - 1)  //  父母兄弟结点 关键字个数 不够
								{
									hebingfeiye(r1, newp, newbro);
						    //可能合并结点完，会发生父母结点关键字总数为0的情况，此情况过于极端，不予考虑，且在m>=5时不会出现
								}
								else  //                   父母兄弟结点 关键字个数 够
								{
									if (r1 == 0)
									{
										temp = newp->parent->key[0];
										newp->parent->key[0] = newbro->key[0];
										for (int i = 1; i < newbro->sumkey; i++)
										{
											newbro->key[i - 1] = newbro->key[i];

										}
										newbro->sumkey--;
										for (int i = n + 1; i < newp->sumkey; i++)
										{
											newp->key[i - 1] = newp->key[i];
										}
										chashuzu(newp->key, newp->sumkey - 1, temp);
									}
									else
									{
										temp = newp->parent->key[r1 - 1];
										newp->parent->key[r1 - 1] = newbro->key[newbro->sumkey - 1];

										newbro->sumkey--;
										for (int i = n + 1; i < newp->sumkey; i++)
										{
											newp->key[i - 1] = newp->key[i];
										}
										chashuzu(newp->key, newp->sumkey - 1, temp);
									}
									break;
								}
							}

						} // while
					}

				}
	}
	else             //  非叶结点    上删除元素   ( 情况4 ） 
	{
	btree* t,*t1;
	int z;
	minshu(p->kid[n + 1], &z, &t);
	shan(shu, z);    //     先删再改
	zhao(*shu, x, &t1);  //防止p指向的空间在上一步被删除
	for (int i = 0; i < t1->sumkey; i++)
	{
		if (x == t1->key[i])
		{
			t1->key[i] = z;
			break;
		}
	}
	
	/*p->key[n] = z;    
	shan(p, z);
	zhao();
	gai;*/               // 此方法有风险，若 p 为根节点进行删除，（根节点可以至少有2个子树）可能会不满足树的定义
	                              //若以 shu 为根节点进行删除，则会删除 x（非叶结点）   
	}

	return 1;
}


