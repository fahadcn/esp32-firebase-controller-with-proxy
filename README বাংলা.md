এই প্রজেক্টটি IoT ডেভেলপমেন্টের বেসিক বিষয়গুলো শেখার জন্য তৈরি করা হয়েছে এবং এটি নতুনদের জন্য সহজভাবে সাজানো। 🔌 এই এমবেডেড সিস্টেমে Proxy Support একটি গুরুত্বপূর্ণ অংশ। যেহেতু আমি চীনে থাকি, তাই Firebase-এর ফ্রি টিয়ার ব্যবহার করার জন্য বেশিরভাগ প্রজেক্টে Proxy Support যুক্ত করেছি। কিছু ক্ষেত্রে কোডের Proxyless Version-ও দেওয়া হবে।

[নোট: ⚠️ অভ্যন্তরীণ ও বাহ্যিক লিংক যেকোনো সময় পরিবর্তিত হতে পারে। এগুলো ব্যবহারের সময় সতর্ক থাকুন।]

🛠️ ব্যবহৃত টুল:
    1. কোডিং: VS Code এবং PlatformIO প্লাগইন
    2. Firebase Spark plan (free tier): https://firebase.google.com/pricing


📁 সব প্রজেক্ট ফাইল "/archive" ফোল্ডারে রাখা আছে। প্রজেক্টগুলো ধারাবাহিকভাবে সাজিয়ে রাখার চেষ্টা করব। কোনো প্রজেক্ট আপলোড ও পরীক্ষা করার জন্য "/archive" থেকে প্রয়োজনীয় ফাইলের কোড কপি করে "src\main.cpp" ফাইলে বসান। নিচে প্রতিটি প্রজেক্টের সংক্ষিপ্ত বর্ণনা দেওয়া হলো।

*\archive\a_1_single_led_control
    💡 একটি LED নিয়ন্ত্রণ করে। Proxy ব্যবহার করা বাধ্যতামূলক নয়, তবে চীন থেকে Firebase API-এর সঙ্গে যোগাযোগ করার জন্য আমার এটি প্রয়োজন হয়েছিল। Proxy হিসেবে আমার লোকাল PC-তে চলা একটি VPN ব্যবহার করা হয়েছে।
     ধাপ ১:
        1. Firebase-এ একটি প্রজেক্ট তৈরি করুন: https://firebase.google.com/pricing
        2. "Authentication"-এ "Users" তৈরি করুন
        3. দেওয়া User ID ব্যবহার করে "Realtime Database"-এর "Rules"-এ read এবং write permission সেট করুন। উদাহরণ:
                { "rules": 
                    { 
                    ".read": "auth.uid === 'IRBYumRJO7W**************'", 
                    ".write": "auth.uid === 'IRBYumRJO7W**************'" 
                    } 
                }
        4. উদাহরণ অনুযায়ী 'Data' import করুন অথবা তৈরি করুন:
            \archive\a_1_single_led_control\led-esp32.json
        5. "Your App" থেকে "SDK setup and configuration" কপি করুন

    ধাপ ২: 🚀
        1. Wi-Fi এবং "SDK setup and configuration" অনুযায়ী .cpp কোড পরিবর্তন করুন
        2. ESP32-তে আপলোড করুন
    ধাপ ৩: 🔄 "Realtime Database"-এর 'Data' value পরিবর্তন করে ESP32-তে পরিবর্তনটি দেখুন। উদাহরণ: 12:0 হলে LED বন্ধ এবং 12:1 হলে LED চালু হবে 💡
