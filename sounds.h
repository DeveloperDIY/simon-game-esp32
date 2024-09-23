extern int pirates_notes[] = {
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,
  
  // NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  // NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  // NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  // NOTE_A4, NOTE_G4, NOTE_A4, REST,
  
  // NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  // NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST,
  // NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
  // NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REST,
  
  // NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  // NOTE_D5, NOTE_E5, NOTE_A4, REST,
  // NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REST,
  // NOTE_C5, NOTE_A4, NOTE_B4, REST,
  
  // NOTE_A4, NOTE_A4,
  //Repeat of first part
  // NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  // NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  // NOTE_A4, NOTE_G4, NOTE_A4, REST,
  
  // NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  // NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  // NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  // NOTE_A4, NOTE_G4, NOTE_A4, REST,
  
  // NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  // NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST,
  // NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
  // NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REST,
  
  // NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  // NOTE_D5, NOTE_E5, NOTE_A4, REST,
  // NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REST,
  // NOTE_C5, NOTE_A4, NOTE_B4, REST,
  // //End of Repeat
  
  // NOTE_E5, REST, REST, NOTE_F5, REST, REST,
  // NOTE_E5, NOTE_E5, REST, NOTE_G5, REST, NOTE_E5, NOTE_D5, REST, REST,
  // NOTE_D5, REST, REST, NOTE_C5, REST, REST,
  // NOTE_B4, NOTE_C5, REST, NOTE_B4, REST, NOTE_A4,
  
  // NOTE_E5, REST, REST, NOTE_F5, REST, REST,
  // NOTE_E5, NOTE_E5, REST, NOTE_G5, REST, NOTE_E5, NOTE_D5, REST, REST,
  // NOTE_D5, REST, REST, NOTE_C5, REST, REST,
  // NOTE_B4, NOTE_C5, REST, NOTE_B4, REST, NOTE_A4
};

extern int pirates_durations[] = {
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,
  
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8,
  
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 8, 4, 8,
  
  // 8, 8, 4, 8, 8,
  // 4, 8, 4, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 4,
  
  // 4, 8,
  //Repeat of First Part
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8,
  
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8,
  
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 8, 4, 8,
  
  // 8, 8, 4, 8, 8,
  // 4, 8, 4, 8,
  // 8, 8, 4, 8, 8,
  // 8, 8, 4, 4,
  // //End of Repeat
  
  // 4, 8, 4, 4, 8, 4,
  // 8, 8, 8, 8, 8, 8, 8, 8, 4,
  // 4, 8, 4, 4, 8, 4,
  // 8, 8, 8, 8, 8, 2,
  
  // 4, 8, 4, 4, 8, 4,
  // 8, 8, 8, 8, 8, 8, 8, 8, 4,
  // 4, 8, 4, 4, 8, 4,
  // 8, 8, 8, 8, 8, 2
};

void playPiratesSong() {
  int size = sizeof(pirates_durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / pirates_durations[note];
    tone(SPEAKER_PIN, pirates_notes[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing:
    noTone(SPEAKER_PIN);
  }
}