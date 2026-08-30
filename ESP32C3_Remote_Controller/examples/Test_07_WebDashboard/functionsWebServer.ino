//================= functionsWebServer ===================
// מגיש את דף האינטרנט (data/) מזיכרון הפלאש - חובה להעלות את תיקיית
// data בנפרד דרך כלי ה-LittleFS Upload (ראו README).
void initWebServer() {
  if (!LittleFS.begin(true)) {
    Serial.println("שגיאה: LittleFS לא עלה - העלית את תיקיית data דרך כלי ה-Upload?");
    return;
  }

  // "no-store" - הוראה חזקה יותר מ-"no-cache": אוסרת על הדפדפן לשמור את
  // הקובץ בכלל, לא רק "לבדוק מול השרת". "no-cache" לבדו לא מנע את
  // בעיית הדפים הישנים אחרי עדכון בפועל.
  const char* NO_CACHE = "no-store, max-age=0";
  httpServer.serveStatic("/", LittleFS, "/index.html", NO_CACHE);
  httpServer.serveStatic("/style.css", LittleFS, "/style.css", NO_CACHE);
  httpServer.serveStatic("/app.js", LittleFS, "/app.js", NO_CACHE);
  httpServer.serveStatic("/board.jpg", LittleFS, "/board.jpg", NO_CACHE);

  httpServer.begin();
  Serial.println("HTTP server started on port 80");
}
//==========================================================
