===============================
САМЫЕ ПОПУЛЯРНЫЕ СИСТЕМНЫЕ ВЫЗОВЫ LINUX
===============================

=== 1. Работа с файлами и вводом/выводом ===
 - open(path, flags, mode)        — открыть файл, вернуть файловый дескриптор
 - creat(path, mode)              — создать новый файл
 - read(fd, buf, count)           — прочитать данные из файла
 - write(fd, buf, count)          — записать данные в файл
 - lseek(fd, offset, whence)      — изменить позицию в файле
 - close(fd)                      — закрыть файловый дескриптор
 - dup(oldfd), dup2(oldfd, newfd) — дублировать файловый дескриптор
pipe(fds[2])                   — создать неименованный канал

=== 2. Работа с файловой системой ===
 - stat(path, struct stat *st)    — получить информацию о файле
 - fstat(fd, struct stat *st)     — то же самое по дескриптору
 - chmod(path, mode)              — изменить права доступа
 - chown(path, uid, gid)          — изменить владельца
 - mkdir(path, mode)              — создать каталог
 - rmdir(path)                    — удалить каталог
 - link(old, new)                 — создать жёсткую ссылку
 - unlink(path)                   — удалить файл
 - rename(old, new)               — переименовать файл
 - access(path, mode)             — проверить права доступа
 - umask(mask)                    — установить маску прав по умолчанию

=== 3. Управление процессами ===
fork()                         — создать новый процесс (копию текущего)
execve(path, argv, envp)       — запустить новую программу
execl(), execv(), execvp()     — разновидности execve()
wait(&status)                  — дождаться завершения потомка
waitpid(pid, &status, options) — дождаться конкретного процесса
exit(status)                   — завершить процесс
_getpid(), getppid()           — PID процесса и родителя
kill(pid, sig)                 — послать сигнал процессу
nice(value), setpriority(...)  — изменить приоритет

=== 4. Работа с памятью ===
brk(addr), sbrk(incr)          — изменить границу сегмента данных
mmap(addr, length, prot, flags, fd, offset) — отобразить файл/память в адресное пространство
munmap(addr, length)           — освободить отображённую область
mprotect(addr, len, prot)      — изменить защиту памяти
msync(addr, len, flags)        — синхронизировать mmap с диском

=== 5. IPC и сеть ===
socket(domain, type, protocol) — создать сокет
bind(fd, addr, len)            — привязать адрес
listen(fd, backlog)            — ожидать соединения
accept(fd, addr, len)          — принять соединение
connect(fd, addr, len)         — подключиться к удалённому хосту
send(fd, buf, len, flags)      — отправить данные
recv(fd, buf, len, flags)      — получить данные
pipe(), dup2(), fork()         — классика IPC
shmget, shmat, shmctl          — разделяемая память (SysV)
semget, semop, semctl          — семафоры (SysV)
msgget, msgsnd, msgrcv         — очереди сообщений (SysV)

=== 6. Время и таймеры ===
time(tloc)                     — получить текущее время
gettimeofday(tv, tz)           — время с микросекундами
clock_gettime(clk_id, tp)      — точное время
nanosleep(req, rem)            — приостановить процесс
alarm(seconds)                 — послать сигнал через N секунд

=== 7. Пользователи и безопасность ===
getuid(), geteuid()            — получить UID и EUID пользователя
getgid(), getegid()            — получить GID и EGID
setuid(uid), setgid(gid)       — сменить идентификаторы
chroot(path)                   — изменить корень файловой системы
setrlimit(resource, rlim)      — ограничить ресурсы процесса

=== 8. Ошибки и диагностика ===
 - errno                          — глобальная переменная с кодом ошибки
 - perror(msg)                    — вывести сообщение об ошибке
 - strerror(errno)                — вернуть строку с описанием ошибки

=== 9. Топ-10 самых часто встречающихся ===
1. open
2. read
3. write
4. close
5. lseek
6. stat
7. fork
8. execve
9. waitpid
10. mmap
