Документ README.md для репозитория Nexus-OPTIC-HUB-UART определяет стандарты хранения телеметрии и структуры супер-лога управляющего ПК под стандарт N-BUS v1.4.
Полный текст спецификации, включая пути сохранения, шаблоны CSV-логов и детальное описание колонок, доступен в исходных документах. Ниже приведена краткая структура:
1.	Спецификация хранения телеметрии:
o	Пути на диске: C:\Users\<Имя_Пользователя>\.openpnp2\nexus_telemetry\head_logs\ (Windows) и ~/.openpnp2/nexus_telemetry/head_logs/ (Linux).
o	Именование сессий: Шаблон nexus_head_rep_YYYYMMDD_HHMMSS.csv для предотвращения раздувания файлов.
o	Формат: Иерархический CSV в кодировке UTF-8 с разделителем ;.
2.	Структурный шаблон супер-лога:
o	Заголовок CSV: Timestamp;Session_ID;DWT_Ticks;Config_Mask;Peak_Amp;Peak_Freq;RMS_Band;R1_Angle;R1_Current;R2_Angle;R2_Current;Z1_Enc_AB;Z1_Enc_Z;Z2_Enc_AB;Z2_Enc_Z;Z3_Enc_AB;Z3_Enc_Z;Z4_Enc_AB;Z4_Enc_Z;Vac_S1;Vac_S2;Vac_S3;Vac_S4;Vac_S5;Vac_S6;Vac_S7;Vac_S8;Valves_Mask_T1;Valves_Mask_T2;Head_Temperature;V_Main_Head;Diag_Flags;CRC_Status.
3.	Навигатор по колонкам:
o	Охватывает системные маркеры времени и ID, акустический и вибрационный домен Edge AI, параметры вращения сопел (оси R), вертикальный ход (оси Z1–Z4), пневматический контроль вакуума и клапанов, а также данные термокомпенсации и самодиагностики.

Nexus-OPTIC-HUB-UART определяет стандарт хранения телеметрии N-BUS v1.4 в формате иерархических CSV-файлов с разделителем ; (UTF-8) для интеграции с OpenPnP и Edge AI.
•	Пути хранения: ОС Windows (C:\Users\<User>\.openpnp2\nexus_telemetry\head_logs\) и Linux (~/.openpnp2/nexus_telemetry/head_logs/).
•	Именование файлов: nexus_head_rep_YYYYMMDD_HHMMSS.csv для каждой новой сессии.
•	Структура заголовка и пример: Полную спецификацию полей (Timestamp, Session_ID, DWT_Ticks, параметры вибрации 0x120, пневматики 0x311 и термокомпенсации 0x410), а также примеры строк можно найти в исходной документации репозитория.

