int fun(int n){
 int count = 0;
 int i;
 for(i =n;i>0;i = i/=2){
 	int j;
  for(j=0;j<i;j++){
   count+=1;
  }
 }
return count;
}
int main(){
	printf("%d",fun(6));
}

