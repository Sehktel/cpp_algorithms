# Расширенные материалы: Fail2ban

## 🚀 Базовое использование

### Установка и настройка

```bash
# Установка
sudo apt-get install fail2ban

# Конфигурация
sudo cp /etc/fail2ban/jail.conf /etc/fail2ban/jail.local

# Редактирование
sudo nano /etc/fail2ban/jail.local
```

### Базовые команды

```bash
# Запуск
sudo systemctl start fail2ban
sudo systemctl enable fail2ban

# Статус
sudo fail2ban-client status

# Статус конкретной тюрьмы
sudo fail2ban-client status sshd

# Разблокировать IP
sudo fail2ban-client set sshd unbanip 192.168.1.100
```

## 📖 Основные концепции

### Компоненты Fail2ban

- **Jail** - конфигурация для сервиса
- **Filter** - правила поиска в логах
- **Action** - действия при обнаружении

### Пример конфигурации

```ini
[sshd]
enabled = true
port = ssh
filter = sshd
logpath = /var/log/auth.log
maxretry = 3
bantime = 3600
```

---

**Примечание:** Это расширенный материал. Основной README содержит структуру темы.


