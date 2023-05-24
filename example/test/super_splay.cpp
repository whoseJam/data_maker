#include<bits/stdc++.h>
using namespace std;

int read(){
	int s=0,f=1;char t=getchar();
	while('0'>t||t>'9'){
		if(t=='-')f=-1;
		t=getchar();
	}
	while('0'<=t&&t<='9'){
		s=(s<<1)+(s<<3)+t-'0';
		t=getchar();
	}
	return s*f;
}
int cnt=0;
template<typename T>
struct Splay_{
	enum{
		BY_ADDRESS=0,
		BY_VALUE=1
	};
	struct SplayNode;
	using Outputer=function<void(T*)>;
	using Compare=function<bool(T*,T*)>;
	using PushFunc=function<void(SplayNode*)>;
	using SingleChangeFunc=function<void(T*)>;
	PushFunc pushDown;
	PushFunc pushUp;
	Compare lessThan;
	Compare equalTo;
	Outputer outputFunc;
	SplayNode* root;
	struct SplayNode{
		SplayNode(T* info){
			this->info=info;
			size=same=1;
			fa=ch[0]=ch[1]=NULL;
		}
		T* info;
		SplayNode* fa;
		SplayNode* ch[2];
		int size;
		int same;
		void InnerPushUp(PushFunc pushUp){
			size=same;
			if(ch[0])size+=ch[0]->size;
			if(ch[1])size+=ch[1]->size;
			pushUp(this);
		}
	};
	Splay_(){
		root=NULL;
		pushDown=[](SplayNode*){};
		pushUp=[](SplayNode*){};
	}
	void rot(SplayNode* x,SplayNode*& tar){
		SplayNode* y=x->fa;
		SplayNode* z=y->fa;
		int L=(y->ch[0])!=x;
		int R=L^1;
		if(y==tar)tar=x;
		else if((z->ch[0])==y)z->ch[0]=x;
		else z->ch[1]=x;
		x->fa=z;y->fa=x;
		if(x->ch[R])x->ch[R]->fa=y;
		y->ch[L]=x->ch[R];x->ch[R]=y;
		y->InnerPushUp(pushUp);
		x->InnerPushUp(pushUp);
	}
	void splay(SplayNode* x,SplayNode*& tar){
		while(x!=tar){
			SplayNode* y=x->fa;
			SplayNode* z=y->fa;
			if(z)pushDown(z);
			pushDown(y);
			pushDown(x); 
			if(y!=tar){
				if((z->ch[1]==y)^(y->ch[1]==x))rot(x,tar);
				else rot(y,tar);
			}
			rot(x,tar);
		}
	}
	SplayNode* Insert(SplayNode*& u,T* info,SplayNode* fa,int type){
		cnt++;
		if(u==NULL){u=new SplayNode(info);u->fa=fa;u->InnerPushUp(pushUp);return u;}
		pushDown(u);SplayNode* ret=NULL;
		if(lessThan(info,u->info))ret=Insert(u->ch[0],info,u,type);
		else if((type==BY_VALUE&&equalTo(info,u->info))||
				(type==BY_ADDRESS&&info==u->info)){ret=u;u->same++;}
		else ret=Insert(u->ch[1],info,u,type);
		u->InnerPushUp(pushUp);
		return ret;
	}
	SplayNode* Insert(T* info,int type){
		SplayNode* x=Insert(root,info,NULL,type);
		splay(x,root);
		return x;
	}
	SplayNode* FindKthMin(SplayNode* u,int k){
		if(u==NULL)return NULL;
		pushDown(u);
		cnt++; 
		int left=(u->ch[0])?u->ch[0]->size:0;
		if(left+1<=k&&k<=left+(u->same))return u;
		if(left>=k)return FindKthMin(u->ch[0],k);
		return FindKthMin(u->ch[1],k-left-(u->same));
	}
	T* FindKthMin(int k){
		return FindKthMin(root,k)->info;
	} 
	SplayNode* FindKthMax(SplayNode* u,int k){
		if(u==NULL)return NULL;
		pushDown(u);
		int right=(u->ch[1])?u->ch[1]->size:0;
		if(right+1==k&&k<=right+(u->same))return u;
		if(right>=k)return FindKthMax(u->ch[1],k);
		return FindKthMax(u->ch[0],k-right-(u->same));
	}
	T* FindKthMax(int k){
		return FindKthMax(root,k)->info;
	}
	SplayNode* Find(SplayNode* u,T* info,int type){
		if(u==NULL)return NULL;
		pushDown(u);
		if(lessThan(info,u->info))return Find(u->ch[0],info,type);
		else if((type==BY_VALUE&&equalTo(info,u->info))||
				(type==BY_ADDRESS&&info==u->info))return u;
		return Find(u->ch[1],info,type);
	}
	SplayNode* FindPrev(){
		SplayNode* u=root->ch[0];
		if(u==NULL)return NULL;
		while(u->ch[1]){
			pushDown(u);
			u=u->ch[1];
		}
		return u;
	}
	SplayNode* FindNext(){
		SplayNode* u=root->ch[1];
		if(u==NULL)return NULL;
		while(u->ch[0]){
			pushDown(u);
			u=u->ch[0];
		}
		return u;
	}
	T* FindPrev(T* info,int type){
		SplayNode* x=Find(root,info,type);
		if(x==NULL)return NULL;
		splay(x,root);
		return FindPrev()->info;
	}
	T* FindNext(T* info,int type){
		SplayNode* x=Find(root,info,type);
		if(x==NULL)return NULL;
		splay(x,root);
		return FindNext()->info;
	}
	void RemoveRoot(){
		root->same--;
		if(root->same==0){
			delete root;
			root=NULL;
		}else root->InnerPushUp(pushUp);
	}
	void RemoveRootChild(int c){
		root->ch[c]->same--;
		if(root->ch[c]->same==0){
			delete root->ch[c];
			root->ch[c]=NULL;
		}else root->ch[c]->InnerPushUp(pushUp);
		root->InnerPushUp(pushUp);
	}
	void RemoveRootRightLeft(){
		SplayNode* tar=root->ch[1]->ch[0];
		tar->same--;
		if(tar->same==0){
			delete tar;
			root->ch[1]->ch[0]=NULL;
		}else root->ch[1]->ch[0]->InnerPushUp(pushUp);
		root->ch[1]->InnerPushUp(pushUp);
		root->InnerPushUp(pushUp);
	}
	void Remove(T* info,int type){
		if(root==NULL)return;
		SplayNode* tar=Find(root,info,type);
		if(tar==NULL)return;
		splay(tar,root);
		SplayNode* prev=FindPrev();
		SplayNode* next=FindNext();
		if(prev==NULL&&next==NULL){RemoveRoot();return;}
		else if(prev==NULL){splay(next,root);RemoveRootChild(0);return;}
		else if(next==NULL){splay(prev,root);RemoveRootChild(1);return;}
		splay(prev,root);splay(next,root->ch[1]);
		RemoveRootRightLeft();
	}
	void ChangeRoot(PushFunc change){
		change(root);
	}
	void ChangeRootChild(int c,PushFunc change){
		change(root->ch[c]);
		root->InnerPushUp(pushUp);
	}
	void ChangeRootRightLeft(PushFunc change){
		change(root->ch[1]->ch[0]);
		root->ch[1]->InnerPushUp(pushUp);
		root->InnerPushUp(pushUp);
	}
	void SingleChange(T* info,PushFunc change){
		if(root==NULL)return;
		SplayNode* tar=Find(root,info);
		if(tar==NULL)return;
		splay(tar,root);
		SplayNode* prev=FindPrev();
		SplayNode* next=FindNext();
		if(prev==NULL&&next==NULL){ChangeRoot(change);return;}
		else if(prev==NULL){splay(next,root);ChangeRootChild(0,change);return;}
		else if(next==NULL){splay(prev,root);ChangeRootChild(1,change);return;}
		splay(prev,root);splay(next,root->ch[1]);
		ChangeRootRightLeft(change);
	}
	void RangeChange(int l,int r,PushFunc change){
		if(root==NULL)return;
		int tot=root->size;
		if(l==1&&r==tot){change(root);return;}
		if(l==1){SplayNode* next=FindKthMin(root,r+1);splay(next,root);ChangeRootChild(0,change);return;}
		if(r==tot){SplayNode* prev=FindKthMin(root,l-1);splay(prev,root);ChangeRootChild(1,change);return;}
		SplayNode* prev=FindKthMin(root,l-1);
		SplayNode* next=FindKthMin(root,r+1);
		splay(prev,root);splay(next,root->ch[1]);
		ChangeRootRightLeft(change); 
	}
	int Rank(T* info,int type){
		if(root==NULL)return -1;
		SplayNode* tar=Find(root,info,type);
		if(tar==NULL)return -1;
		splay(tar,root);
		if(root->ch[0]==NULL)return 1;
		return tar->ch[0]->size+1;
	}
	int Rank(SplayNode* u){
		splay(u,root);
		if(root->ch[0]==NULL)return 1;
		return root->ch[0]->size+1;
	}
	T* FindMin(){
		if(root==NULL)return NULL;
		SplayNode* u=root;
		while(u->ch[0]!=NULL){
			pushDown(u);
			u=u->ch[0];
		}
		return u->info;
	}
	T* FindMax(){
		if(root==NULL)return NULL;
		SplayNode* u=root;
		while(u->ch[1]!=NULL){
			pushDown(u);
			u=u->ch[1];
		}
		return u->info;
	}
	void Output(SplayNode* x){
		if(x==NULL)return;
		pushDown(x);
		Output(x->ch[0]);
		outputFunc(x->info);
		Output(x->ch[1]);
	}
	void Output(){
		Output(root);
		cout<<"\n";
	}
};

const int N=300005;
struct VInfo{
	int rev,val;
}a[N];
Splay_<VInfo> tr;
bool cmp(VInfo*a,VInfo*b){return a->val<b->val;}
int eq(VInfo*a,VInfo*b){return a->val==b->val;}
void pushRev(Splay_<VInfo>::SplayNode* u){
	if(u!=NULL){
		u->info->rev^=1;
		swap(u->ch[0],u->ch[1]);
	}
}
void pushDown(Splay_<VInfo>::SplayNode* u){
	if(u->info->rev){
		pushRev(u->ch[0]);
		pushRev(u->ch[1]);
		u->info->rev^=1;
	}
}
void output(VInfo* a){
	cout<<a->val<<" ";
}
int R(int l,int r){
	return ((rand())%(r-l+1)+l);
}
