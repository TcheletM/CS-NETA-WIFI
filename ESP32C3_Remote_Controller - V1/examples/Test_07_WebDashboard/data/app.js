//================== app.js ==================
// מתחבר ל-WebSocket Server על אותו ESP32 (פורט 81), מאזין להודעות CTRL:{...}
// שמגיעות מהשלט (ESP32C3042), ומצייר אותן בזמן אמת.

const WS_PORT = 81;
const wsUrl = `ws://${location.hostname}:${WS_PORT}/`;

// טווח נורמליזציה של ערך ה-tilt הגולמי (raw accel X) לזווית ציור (-90..90 מעלות).
// כוונו את הערך הזה לפי מה שאתם רואים בפועל ב"ערך גולמי" מתחת למד ההטיה.
const TILT_RANGE = 16384; // ±1g בטווח ברירת המחדל של ה-MPU6050 (±2g)

const statusEl    = document.getElementById('status');
const tiltValueEl = document.getElementById('tiltValue');
const canvas      = document.getElementById('horizon');
const ctx         = canvas.getContext('2d');

// סוללה
const batteryReading = document.getElementById('batteryReading');
const batteryBar = document.getElementById('batteryBar');
const batteryStatus = document.getElementById('batteryStatus');

// פאנל "כפתורים" (D-pad) מייצג את הג'ויסטיק (4 כיוונים + לחיצה) -
// לא מקושר לכפתורי KEY1/KEY2 הפיזיים (אלה מוצגים בנפרד במיפוי השלט).
const dpad = {
  left:   document.getElementById('btnLeft'),
  right:  document.getElementById('btnRight'),
  up:     document.getElementById('btnUp'),
  down:   document.getElementById('btnDown'),
  select: document.getElementById('btnSelect'),
};
const attackEl = document.getElementById('attackIndicator');

// אלמנטים של מיפוי השלט (מדמה את פריסת הלוח הפיזי)
const joystickKnob = document.getElementById('joystickKnob');
const oledMockup    = document.getElementById('oledMockup');
const motorVibration = document.getElementById('motorVibration');
const key1El = document.getElementById('key1');
const key2El = document.getElementById('key2');
const ledDots = [
  document.getElementById('led0'),
  document.getElementById('led1'),
  document.getElementById('led2'),
  document.getElementById('led3'),
  document.getElementById('led4'),
];

let ws;

function connect() {
  ws = new WebSocket(wsUrl);

  ws.onopen = () => {
    statusEl.textContent = 'מחובר';
    statusEl.className = 'status connected';
    oledMockup.textContent = 'WS Linked';
  };

  ws.onclose = () => {
    statusEl.textContent = 'מנותק - מנסה שוב...';
    statusEl.className = 'status disconnected';
    oledMockup.textContent = 'OLED';
    setTimeout(connect, 1500);
  };

  ws.onerror = () => ws.close();

  ws.onmessage = (event) => {
    const msg = event.data;
    if (typeof msg !== 'string' || !msg.startsWith('CTRL:')) return;

    let data;
    try {
      data = JSON.parse(msg.substring(5));
    } catch (e) {
      return;
    }

    handleControlData(data);
  };
}

function handleControlData(d) {
  tiltValueEl.textContent = d.tilt;
  drawHorizon(d.tilt);

  setButtonState(dpad.left, d.joyLeft);
  setButtonState(dpad.right, d.joyRight);
  setButtonState(dpad.up, d.joyUp);
  setButtonState(dpad.down, d.joyDown);
  setButtonState(dpad.select, d.select);

  if (d.attack) {
    attackEl.classList.add('active');
    setTimeout(() => attackEl.classList.remove('active'), 200);
  }

  // עדכון סוללה
  if (d.battery !== undefined) {
    updateBattery(d.battery);
  }

  updateControllerMockup(d);
}

function updateBattery(voltage) {
  // הצגת המתח כמו מודד
  batteryReading.textContent = voltage.toFixed(2) + 'V';

  // חישוב צבע gradient חלק מאדום לירוק
  let color;
  const minVoltage = 2.7;
  const maxVoltage = 4.2;
  const normalizedVoltage = (voltage - minVoltage) / (maxVoltage - minVoltage);

  if (normalizedVoltage < 0.25) {
    // אדום
    color = '#dc2626';
  } else if (normalizedVoltage < 0.4) {
    // אדום לכתום
    const t = (normalizedVoltage - 0.25) / 0.15;
    color = `rgb(${Math.round(239 - t * 49)}, ${Math.round(68 + t * 59)}, ${Math.round(68 - t * 68)})`;
  } else if (normalizedVoltage < 0.6) {
    // כתום לצהוב
    const t = (normalizedVoltage - 0.4) / 0.2;
    color = `rgb(${Math.round(249 - t * 32)}, ${Math.round(115 + t * 107)}, ${Math.round(22 - t * 22)})`;
  } else if (normalizedVoltage < 0.85) {
    // צהוב לירוק
    const t = (normalizedVoltage - 0.6) / 0.25;
    color = `rgb(${Math.round(234 - t * 212)}, ${Math.round(179 + t * 76)}, ${Math.round(8 + t * 76)})`;
  } else {
    // ירוק
    color = '#16a34a';
  }
  batteryReading.style.color = color;

  // עדכון אורך הבר (יחסי לטווח 2.7V עד 4.2V)
  const percentage = Math.max(0, Math.min(100, ((voltage - minVoltage) / (maxVoltage - minVoltage)) * 100));
  batteryBar.style.width = percentage + '%';

  // הודעת סטטוס
  if (voltage >= 4.0) {
    batteryStatus.textContent = '✓ טעונה';
  } else if (voltage >= 3.7) {
    batteryStatus.textContent = '⚠ בסדר';
  } else if (voltage >= 3.4) {
    batteryStatus.textContent = '⚠ נמוכה';
  } else {
    batteryStatus.textContent = '⛔ קרובה לחלוטין';
  }
}

function setButtonState(el, active) {
  el.classList.toggle('active', !!active);
}

// מעדכן את פאנל "מיפוי השלט" - מדמה מה שקורה בפועל על הלוח הפיזי.
// כל השדות (btnLeft/btnRight/joyLeft/joyRight/joyUp/joyDown/motor/led)
// מגיעים ישירות מהשלט - לא קירוב. הג'ויסטיק והכפתורים עצמאיים לחלוטין
// זה מזה, כמו בחומרה בפועל.
function updateControllerMockup(d) {
  const nx = d.joyLeft ? 1 : (d.joyRight ? -1 : 0); // הפוך ל-ny - מאומת ויזואלית מול המסך (ראה הערה למעלה)
  const ny = d.joyUp ? -1 : (d.joyDown ? 1 : 0);
  const maxOffset = 35; // פיקסלים - הוגדל מ-10 כי זה היה קטן מדי לראות כיוון בבירור
  joystickKnob.style.transform = `translate(${nx * maxOffset}px, ${ny * maxOffset}px)`;

  key1El.classList.toggle('active', !!d.btnLeft);
  key2El.classList.toggle('active', !!d.btnRight);

  // מצב המנוע האמיתי מהשלט (0/1) - זהה בדיוק למה שקורה על MOTOR_PIN בפועל
  if (motorVibration) {
    motorVibration.classList.toggle('active', !!d.motor);
  }

  // מצב הלדים האמיתי מהשלט: 0=כבוי, 1=שמאלה(אדום), 2=ימינה(כחול), 3=למעלה(ירוק), 4=למטה(צהוב)
  // כל 4 הלדים נדלקים יחד באותו צבע - בדיוק כמו strip.fill() בפועל על השלט.
  const ledState = d.led || 0;
  const LED_COLORS = { 1: '#ef4444', 2: '#3b82f6', 3: '#22c55e', 4: '#eab308' };
  const color = LED_COLORS[ledState];

  ledDots.forEach((dot) => {
    dot.classList.remove('lit');
    dot.style.color = '';
    dot.style.background = '';
  });
  if (color) ledDots.forEach((dot) => {
    dot.classList.add('lit');
    dot.style.color = color;
    dot.style.background = color;
  });
}

function drawHorizon(tilt) {
  // מינוס - כדי שכיוון הסיבוב יתאים לכיוון ההטיה האמיתי בפועל (היה הפוך)
  const angle = Math.max(-90, Math.min(90, -(tilt / TILT_RANGE) * 90));
  const rad = (angle * Math.PI) / 180;

  const w = canvas.width, h = canvas.height;
  const cx = w / 2, cy = h / 2;
  const r = Math.min(w, h) / 2 - 6;

  ctx.clearRect(0, 0, w, h);

  // מסגרת חיצונית
  ctx.beginPath();
  ctx.arc(cx, cy, r, 0, Math.PI * 2);
  ctx.strokeStyle = '#3a4a5c';
  ctx.lineWidth = 3;
  ctx.stroke();

  ctx.save();
  ctx.beginPath();
  ctx.arc(cx, cy, r - 2, 0, Math.PI * 2);
  ctx.clip();

  ctx.translate(cx, cy);
  ctx.rotate(rad);

  // שמיים (למעלה) וקרקע (למטה) - סגנון "אופק מלאכותי" (כמו במכשירי טיסה)
  ctx.fillStyle = '#2b6cb0';
  ctx.fillRect(-r * 2, -r * 2, r * 4, r * 2);
  ctx.fillStyle = '#8a5a2b';
  ctx.fillRect(-r * 2, 0, r * 4, r * 2);

  // קו האופק
  ctx.strokeStyle = '#ffffff';
  ctx.lineWidth = 2;
  ctx.beginPath();
  ctx.moveTo(-r * 2, 0);
  ctx.lineTo(r * 2, 0);
  ctx.stroke();

  ctx.restore();

  // סמן מרכז קבוע (לא מסתובב) - מייצג את השלט עצמו
  ctx.strokeStyle = '#ffd23f';
  ctx.lineWidth = 3;
  ctx.beginPath();
  ctx.moveTo(cx - 20, cy);
  ctx.lineTo(cx - 6, cy);
  ctx.moveTo(cx + 6, cy);
  ctx.lineTo(cx + 20, cy);
  ctx.stroke();
}

drawHorizon(0); // מצב התחלתי - מרכז, לפני חיבור
updateControllerMockup({ btnLeft: false, btnRight: false, joyLeft: false, joyRight: false, joyUp: false, joyDown: false, select: false, tilt: 0, attack: false, motor: false, led: 0 });
connect();
