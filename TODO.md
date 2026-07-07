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

## ⚫ Norm (norminette) — 36/36 dosya hatalı, 270 hata (taze tarama: 2026-07-08)

> Not: Yukarıdaki "34/34, ~1192 hata" sayısı eski bir tarama; aradan geçen
> zamanda (bu oturumdaki düzeltmeler dahil) hata sayısı belirgin biçimde
> düştü. Aşağısı güncel durum.

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
- [ ] **Tüm dosyalarda tab/space düzeltmesi** — artık çok daha az (`SPACE_REPLACE_TAB`
      25, `TOO_FEW_TAB` 1), ama hâlâ kalan yerler var.
- [ ] **42 header eksik** — 36 dosyanın 36'sında da yok. Her `.c`/`.h` dosyasının
      başına standart 42 header eklenmeli.
- [ ] Dosya başına 5 fonksiyon limitini aşan dosyalar (`TOO_MANY_FUNCS`, 32 ihlal
      genelinde): `exec_cmd.c` (7 fazla bildirim), `expand_utils.c` (8),
      `parser.c` (7 — 25-satır bölmesi sırasında bilinçli olarak arttı, ayrı
      dosyaya taşınmadı), `signals.c` (4), `ft_export.c` (3), `exec_heredoc.c`
      (2), `parser_utils.c` (1 — `find_right_pipe`, `parser_redir.c`'ye
      taşınabilir, orada 1 fonksiyonluk yer var).
- [ ] `expand/expand_utils.c` içindeki 3 ternary ifadesini (satır 79, 80, 95)
      if/else'e çevir (norm'da ternary yasak).
- [ ] Değişken tanımlarındaki hizalama hatalarını düzelt (9 yer,
      `MISALIGNED_VAR_DECL`).
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
      değil. Silinmeli ya da `tests/` gibi ayrı bir klasöre taşınmalı.
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
