// redirection için önce fd alması lazım 
// 0->stdin,1->stdout,2->stder
// örneğin echo hello 
// aslında stdout yazıyor 


//dup2(fd,1);
//stdout artık ekranda değil fd nereye bağlıysa oraya gitsin demek istiyor.
