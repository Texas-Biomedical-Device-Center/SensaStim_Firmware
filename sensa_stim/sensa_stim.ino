
bool new_stim = false;
int stim_booth_number = -1;

int num_vns_outputs = 10;
int vns_output_pins[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
unsigned long vns_millis[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int vns_millis_duration = 5;

int num_led_outputs = 10;
int led_output_pins[10] = { 23, 26, 29, 32, 35, 38, 41, 47, 44 };
unsigned long led_millis[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int led_millis_duration = 1000;

int num_button_inputs = 10;
int button_input_pins[10] = { 22, 25, 28, 31, 34, 37, 40, 46, 43 };

unsigned long button_refractory_period = 10000;
unsigned long last_button_press_times[10] = { -10000, -10000, -10000, -10000, -10000, -10000, -10000, -10000, -10000, -10000 };


void setup ()
{
    for (int i = 0; i < num_vns_outputs; i++)
    {
        pinMode(vns_output_pins[i], OUTPUT);
    }

    for (int i = 0; i < num_led_outputs; i++)
    {
        pinMode(led_output_pins[i], OUTPUT);
    }

    for (int i = 0; i < num_button_inputs; i++)
    {
        pinMode(button_input_pins[i], INPUT_PULLUP);
    }
    
    Serial.begin(115200);
}

void loop ()
{
    //Check each button to see if it has been pressed
    for (int i = 0; i < num_button_inputs; i++)
    {        
        int button_read = digitalRead(button_input_pins[i]);
        button_read = !button_read;

        unsigned long current_millis = millis();

        if (button_read)
        {
            if (current_millis >= (last_button_press_times[i] + button_refractory_period))
            {
                last_button_press_times[i] = current_millis;
                Serial.println(i);

                //Raise the VNS pin high
                digitalWrite(vns_output_pins[i], HIGH);
                vns_millis[i] = current_millis;
    
                //Indicate that VNS is occurring using the LED
                digitalWrite(led_output_pins[i], HIGH);
                led_millis[i] = current_millis;
            }
        }
    }

    //Get the current time
    unsigned long current_time = millis();
    
    //Turn off LEDs as necessary
    for (int i = 0; i < num_led_outputs; i++)
    {
        if (current_time > (led_millis[i] + led_millis_duration))
        {
            digitalWrite(led_output_pins[i], LOW);
        }
    }

    //Stop any VNS triggers as necessary
    for (int i = 0; i < num_vns_outputs; i++)
    {
        if (current_time > (vns_millis[i] + vns_millis_duration))
        {
            digitalWrite(vns_output_pins[i], LOW);
        }
    }
}
