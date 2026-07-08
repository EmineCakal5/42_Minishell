# Minishell - Tamamlanacaklar Listesi

WSL üzerinde derleyip gerçek testler ve norminette ile yapılan analize dayanıyor.
Öncelik sırasına göre: Kritik → Fonksiyonel → Norm → Küçük/Kozmetik → Dosya/README.

## 🔴 Kritik (mandatory şartı geçersiz kılıyor)

- [x] **Heredoc tamamen bozuk** — `executor/exec_redir.c` satır 28-31. **DÜZELTİLDİ.**
      `dup2` hedefi artık `REDIR_IN || HEREDOC` için `STDIN_FILENO` oluyor.
      WSL'de yeniden derlenip hem düz heredoc (`cat << EOF ... EOF`) hem de
      heredoc+output-redirect kombinasyonu (`cat << EOF > dosya ... EOF`) ile
      doğrulandı; ASan/UBSan ile regresyon koşuldu, hata/leak yok.
      **Not (yeni bulunan, henüz düzeltilmedi):** heredoc bir pipe'ın sağ
      tarafındaysa (örn. `echo a | cat << EOF`), heredoc okuma satırları
      terminalden değil pipe'ın kendisinden okunuyor — çünkü `exec_pipe.c`
      stdin'i pipe'a bağladıktan SONRA `open_heredoc()` çağrılıyor ve o da
      mevcut stdin'den (yani pipe'tan) okuyor. Bu ayrı bir kök nedenli,
      düşük öncelikli bir edge-case; kritik madde kapsamında değildi,
      dokunulmadı.

## 🟠 Fonksiyonel hatalar

- [x] **`export` çoklu argümanı desteklemiyor** — `builtins/ft_export.c`. **DÜZELTİLDİ.**
      Tekil satır artık `export_one()` yardımcı fonksiyonuna çıkarıldı, `ft_export`
      `cmd->args[1..]` üzerinde döngü kurup her argümanı ayrı ayrı işliyor.
      `export A=1 B=2 C=3` artık üçünü de doğru set ediyor (WSL'de derlenip
      `env | grep` ile ve ASan/UBSan ile doğrulandı, leak/hata yok).
- [x] **`export DEGISKEN` (değersiz) bozuk env kaydı üretiyor** — `builtins/ft_export.c`.
      **DÜZELTİLDİ** (dosyada `build_entry()` yardımcı fonksiyonu ile `=`
      işareti yoksa otomatik ekleniyor; `export NOVALUE` artık `NOVALUE=`
      şeklinde doğru kayıt oluşturuyor — `env | grep` ile doğrulandı).
- [x] **Heredoc + Ctrl-C** — `signals/signals.c`, `executor/exec_heredoc.c`. **DÜZELTİLDİ.**
      Heredoc satırları artık `readline()` yerine kendi `read()` tabanlı
      okuyucumuzla (`read_heredoc_line`) okunuyor. Ctrl-C'ye özel bir SIGINT
      handler (`handle_sigint_heredoc`, `SA_RESTART` olmadan) `g_exit_status`'u
      130 yapıyor; bloke olan `read()` bunu EINTR ile hemen kesiyor, döngü
      iptali algılayıp geçici dosyayı silip komutu hiç çalıştırmadan `$?`'yi
      130 yaparak prompt'a dönüyor (bash'teki gibi).
      Önce `readline()` ile aynısını denedim ama GNU Readline'ın kendi SIGINT
      iç mekanizması, kesintiden hemen sonraki ilk satırın baştan 2 karakterini
      tutarlı biçimde yutuyordu (`rl_catch_signals`/`rl_done` kombinasyonlarının
      hiçbiri düzeltmedi) — bu yüzden heredoc okuması tamamen ham `read()`'e
      taşındı. Subject zaten heredoc'un history güncellemesi gerekmediğini
      belirtiyor, yani readline'a ihtiyaç yoktu.
      pty tabanlı testlerle doğrulandı: iptal sonrası hang yok, ilk komut dahil
      hiçbir karakter kaybı yok, `$?`=130, iptalden sonraki normal heredoc'lar
      ve komutlar sorunsuz çalışıyor. Tam regresyon ASan/UBSan ile temiz.
- [ ] **Process group / job control eksik** — hiçbir yerde `setpgid` çağrılmıyor.
      Ön planda komut çalışırken ana shell de aynı SIGINT'i alıp ekrana
      anlamsız fazladan prompt basıyor. (Ctrl-C testinde gözlemlendi.)
- [ ] `cd` hata mesajında yazım hatası: `builtins/ft_cd.c:60` → `"minshell: cd"`
      olmalı `"minishell: cd"`.

## ⚫ Norm (norminette) — 47/47 dosya hatalı, 190 hata (taze tarama: 2026-07-09,
   dosya sayısı `TOO_MANY_FUNCS` bölünmesiyle 36'dan 47'ye çıktı)

> Not: Yukarıdaki "34/34, ~1192 hata" sayısı eski bir tarama; aradan geçen
> zamanda (bu oturumdaki düzeltmeler dahil) hata sayısı belirgin biçimde
> düştü. Aşağısı güncel durum. Bu oturumda gerçek `norminette` (WSL'de
> `pip install --break-system-packages norminette` ile kuruldu) kullanılarak
> doğrulandı, artık tahmine dayanmıyor.

- [x] **25 satırı aşan fonksiyonlar (kısmen)** — `parser/parser.c`
      (`create_node_cmd`, `parse_rec`, `print_ast`), `tokenizer/token.c`
      (`tokenize`) ve `utils/utils.c` (`ft_substr`) davranış değiştirilmeden
      yardımcı fonksiyonlara bölündü. `tokenize()`'daki `TOO_MANY_VARS_FUNC`
      (6 değişken) de bu sırada düzeldi; yeni `handle_op_token`/
      `handle_word_token` fonksiyonları `tokenizer/token_make.c`'ye taşındı
      (Makefile + `tokenizer.h` güncellendi, `token.c`'nin fonksiyon sayısı
      norm sınırını aşmasın diye).
      **Kalan 13 ihlal (8 dosya):** `expand_utils.c` (4), `exec_cmd.c` (2),
      `main.c` (2), `exec_redir.c`, `exec_pipe.c`, `exec_heredoc.c`,
      `ft_unset.c`, `test_tokenizer.c` (1'er).
- [x] `parser/parser.c` içindeki (eski satır ~152) `for` döngüsü (`print_ast`
      içinde) `while`'a çevrildi. **Not:** fonksiyon silinmedi, norm'a uygun
      hale getirildi; hâlâ hiçbir yerden çağrılmıyor — silme kararı aşağıdaki
      "Küçük/kozmetik" maddesine bağlı.
- [x] **Tüm dosyalarda tab/space düzeltmesi** — **DÜZELTİLDİ.** WSL'e kurulan
      `norminette` ile tam tarama yapılıp `SPACE_REPLACE_TAB`/`SPACE_BEFORE_FUNC`/
      `MIXED_SPACE_TAB`/`TOO_FEW_TAB` kategorisindeki 30 ihlalin tümü tek tek
      giderildi: fonksiyon dönüş tipi ile ismi arasında tab kullanılmayan ~20
      fonksiyon tanımı (`ft_echo`, `ft_env`, `ft_exit`, `ft_export`, `ft_pwd`,
      `ft_unset`, `is_builtin`, `run_builtin`, `execute`, `exec_cmd`,
      `open_heredoc`, `apply_redirections`, `handle_sigint`, `setup_signals`,
      `setup_signals_child`, `reset_signals`, `restore_signals` vb.), birkaç
      değişken bildirimi (`ft_echo.c`, `ft_pwd.c`, `exec_redir.c`), `executor.h`/
      `utils.h`'deki tab'sız prototipler, `minishell.h:50`'deki fazladan boşluk
      (`} \tt_redir;` → `}\tt_redir;`) ve `token_control.c:35`'teki
      boşluk+tab karışık devam satırı girintisi. Sonrasında tam tarama:
      bu 4 kategori 30 → 0. WSL'de `-Wall -Wextra -Werror` ile hatasız derlendi.
- [ ] **42 header eksik** — 36 dosyanın 36'sında da yok. Her `.c`/`.h` dosyasının
      başına standart 42 header eklenmeli.
- [x] **Dosya başına 5 fonksiyon limitini aşan dosyalar (`TOO_MANY_FUNCS`)** — **DÜZELTİLDİ.**
      Her dosya, aynı klasörde birbiriyle doğrudan ilişkili (biri diğerini
      çağıran) fonksiyon grupları halinde yeni dosyalara bölündü, davranış
      değiştirilmedi:
      - `executor/exec_cmd.c` (12→5) → `executor/exec_path.c` (PATH arama:
        `find_cmd_path`/`find_cmd_in_path`/`join_cmd_path`/`get_path_dirs`) +
        `executor/exec_path_utils.c` (PATH string yardımcıları: `join_path`/
        `split_path_dirs`/`free_path_list`).
      - `expand/expand_utils.c` (13) → `expand/expand_value.c` (asıl
        `expand_value` motoru), `expand/expand_env.c` (env/sayı yardımcıları:
        `env_value`/`int_to_string`/`get_var_name`), `expand/expand_append.c`
        (string ekleme ilkel fonksiyonları). Eski `expand_utils.c` silindi.
      - `parser/parser.c` (12→4) → `parser/parser_cmd.c` (tek komut node'u
        kurma: `create_node_cmd` ve yardımcıları) + `parser/parser_debug.c`
        (kullanılmayan `print_ast`/`print_cmd_node`/`print_redirs` debug
        zinciri).
      - `parser/parser_utils.c` (6→4) → `parser/parser_pipe.c` (`go_end` +
        `find_right_pipe`, zaten birbirini çağıran tek çift — `parser_redir.c`
        yerine ayrı dosyaya alındı çünkü `go_end` sadece `find_right_pipe`
        tarafından kullanılıyor, redirection mantığıyla ilgisi yok).
      - `signals/signals.c` (9→4) → `signals/signals_heredoc.c` (heredoc'a
        özel SIGINT: `handle_sigint_heredoc`/`reset_signals`/`restore_signals`)
        + `signals/signals_term.c` (terminal process group: `get_terminal_pgrp`/
        `set_terminal_pgrp`).
      - `builtins/ft_export.c` (8→3) → `builtins/ft_export_utils.c` (`key_match`/
        `shell_strdup`/`build_entry`/`export_key_len`/`export_env_count`; son
        ikisi `ft_unset.c`/`main.c` içindeki aynı isimli `static` fonksiyonlarla
        çakışmasın diye `export_` önekiyle yeniden adlandırıldı).
      - `executor/exec_heredoc.c` (7→3) → `executor/exec_heredoc_io.c`
        (geçici dosya adı üretimi + büyüyen tamponla satır okuma:
        `build_heredoc_name`/`read_heredoc_line` ve yardımcıları).
      Makefile ve ilgili `.h` dosyaları (`executor.h`, `expand.h`, `parser.h`,
      `signals.h`, `builtins.h`) güncellendi. WSL'de `-Wall -Wextra -Werror`
      ile tüm dosyalar hatasız derlendi, link edilip `echo hello world` ile
      çalıştırıldı (yerel `libreadline` dev paketiyle); davranışta değişiklik
      yok.
- [ ] `expand/expand_append.c` içindeki 3 ternary ifadesini (satır 9, 10, 25 —
      dosya bu oturumda eski `expand_utils.c`'den ayrıldı, ternary'ler oraya
      taşındı) if/else'e çevir (norm'da ternary yasak).
- [x] **Değişken tanımlarındaki hizalama hatalarını düzelt (`MISALIGNED_VAR_DECL`)** —
      **DÜZELTİLDİ.** `pip`/venv olmadığı için `get-pip.py --break-system-packages`
      ile kurulan `norminette` paketiyle (WSL) tam tarama yapılıp 9 ihlal tek
      tek konumlandırıldı ve tab hizalaması norm'un beklediği sütuna göre
      düzeltildi:
      - `builtins/ft_unset.c:43-45` — `int i;`/`int j;`/`char **new_env;`.
      - `executor/executor.h:8-9` ve `utils/utils.h:8-10` — art arda gelen
        `typedef struct s_x t_x;` blokları (alias isimleri hizasız).
      - `executor/exec_pipe.c:16-18` (bu oturumda `exec_cmd.c`'den ayrılan
        dosya) — `pid_t pid1/pid2/old_pgrp;` bloğu.
      - `executor/exec_heredoc_io.c:27-28` (bu oturumda yeni oluşturulan
        dosya) — `size_t index;` / `static unsigned long counter;` bloğu.
      Düzeltme sonrası tam tarama: `MISALIGNED_VAR_DECL` sayısı 9 → 0. WSL'de
      `-Wall -Wextra -Werror` ile tekrar derlenip hatasız derlendi doğrulandı.
- [ ] En çok hata veren dosyalar (bugünkü tarama, öncelik sırası): `exec_cmd.c`
      (50), `expand_utils.c` (21), `minishell.h` (17), `exec_redir.c` (16),
      `exec_pipe.c` (15), `signals.c` (12), `ft_unset.c` (10).

## 🟡 Küçük / kozmetik

- [ ] `echo -n -n hello` — sadece ilk `-n` flag'i tanınıyor; bash art arda
      tekrarları da kabul ediyor. (Subject'in gerektirdiği minimum karşılanıyor,
      isteğe bağlı iyileştirme.)
- [ ] Ölü dosya `executor/exec_re.c` — sadece yorum satırları, Makefile'da yok,
      hiç derlenmiyor. Silinmeli.
- [ ] `test_tokenizer.c` (proje kökünde) — geliştirme kalıntısı, build'e dahil
      değil. Silinmeli ya da `tests/` gibi ayrı bir klasöre 
      taşınmalı.
- [ ] `print_ast()` (`parser/parser.c` + `parser.h`) — hiçbir yerden çağrılmıyor,
      debug amaçlı kalmış. Norm ihlali (`for` döngüsü) artık düzeltildi, o
      yüzden norm için silinmesi zorunlu değil; yine de ölü kod olarak
      istenirse silinebilir.

## 📄 README.md

- [ ] Dosya **UTF-16** olarak kayıtlı — UTF-8'e çevrilmeli (şu an GitHub'da/çoğu
      araçta karakterler arası null byte'lardan dolayı bozuk görünüyor).
- [ ] İlk satır subject'in zorunlu kıldığı italik cümle değil:
      *"This project has been created as part of the 42 curriculum by \<login1>[, \<login2>...]."*
      eklenmeli.
- [ ] İçerik güncel değil: "Executor: pending", "Builtins: pending",
      "Signal handling: Not implemented" yazıyor ama hepsi çalışıyor —
      gerçek duruma göre güncellenmeli.
- [ ] "Resources" bölümünde AI kullanımının nasıl/ne için kullanıldığı
      açıklanmalı (subject'in zorunlu istediği kısım).

## ✅ Zaten doğru çalışan (dokunmaya gerek yok)

Komut çalıştırma + PATH/göreli/mutlak yol arama, çok aşamalı pipe'lar,
`<`/`>`/`>>` redirection, tek/çift tırnak + `$VAR`/`$?` expansion, `cd`/`pwd`/
`unset`/`env`/`exit` builtin'leri, exit code yayılımı (127, exit N, sinyal+128),
Ctrl-C/Ctrl-D/Ctrl-\ davranışı, bozuk syntax'ta çökmeden "syntax error" verilmesi.
ASan/UBSan ile test edilen tüm bu akışlarda memory leak/hata bulunmadı.

## Bonus (mandatory %100 tamamlanmadan değerlendirilmiyor)

- [ ] `&&` / `||` operatörleri (parantez ile öncelik desteği dahil)
- [ ] `*` wildcard (yalnızca çalışma dizini için)
