#include "../minishell.h"
#include "signals.h"

//prompt satırında ctrl+c yapınca ne olacağını tanımlar
static void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}
void setup_signals(void)
{
   struct sigaction sa;

   //1. sigint (Ctrl+c) ayarı
   sa.sa_handler = handle_sigint;//sinyal geldiğinde hangi fonknun çalışacağını belirtir
   sigemptyset(&sa.sa_mask);//Sinyal işlenirken sa_mask içerisindeki diğer sinyalleri bloke eder. biz boş bırakarak herhangi bir bloklama yapmıyoruz.
   sa.sa_flags = SA_RESTART;// eğer readline beklerken sinyal gelirse, readline fonk.nun yarıda kesilip hata vermesini engeller ve otomatik olarak devam etmesini sağlar
   sigaction(SIGINT, &sa, NULL); // signal() fonk.nun yaptığı işi bu sistem çağrısı daha kararlı bir şekilde gerçekleştirir

   //2. SIGQUIT (Ctrl+\) ayarı (yoksay)
   sa.sa_handler = SIG_IGN;
   sigaction(SIGQUIT, &sa, NULL);
}

void setup_signals_child(void)
{
    //çocuk süreçler shellin özel sinyal davranışlarını devralmamalıdır
    //varsayılan(default) davranışa (SIG_DFL) dönerler
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

}
void reset_signals(void)
{
    struct sigaction sa;
    
    //Heredoc okurken Ctrl+C basılırsa shelln normal davranmasını değil,
    //o anki okuma işlemini sonlandırıp çıkmasını istiyoruz.
    //Bu yüzden burada sinyalleri farklı bir handler'a bağlayabilirz
    //veya basitçe varsayılan davranışı tetikleyebilirz

    sa.sa_handler = SIG_IGN;//Örnek= heredoc sırasında ctrl+c yi yoksayabiliriz
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}