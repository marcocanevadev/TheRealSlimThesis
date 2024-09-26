class Osc
{
    public:
    Osc(){}
    ~Osc(){}

    float y = 0;
    private:

    
};
class NaiveOscillator
{
public:
	NaiveOscillator(float defaultFrequency = 440.0, int defaultWaveform = 0)
	{
		waveform = defaultWaveform;
		frequency = defaultFrequency;
	}

	~NaiveOscillator() {}

	void prepareToPlay(float sampleRate)
	{
		
		samplePeriod = 1.0f / sampleRate;
	}

	void setFrequency(float newValue)
	{
		// No zero-frequency allowed
		frequency = newValue;
	}

	void setWaveform(int newValue)
	{
		waveform = newValue;
	}

	void getNextAudioBlock(float buffer, const int numsamples)
	{
		
		

	}

	float getNextAudioSample()
	{
		auto sampleValue = 0.0;

		switch (waveform)
		{
		case 0: // Sinusoidale
			sampleValue = sin(TWO_PI * currentPhase);
			break;
		case 1: // Triangular
			sampleValue = 4.0 * abs(currentPhase - 0.5) - 1.0;
			break;
		case 2: // Saw UP
			sampleValue = 2.0 * currentPhase - 1.0;
			break;
		case 3: // Saw down
			sampleValue = -2.0 * currentPhase + 1.0;
			break;
		case 4: // Square
			sampleValue = (currentPhase > 0.5) - (currentPhase < 0.5);
			break;
		default:
			// WTF MAN!
	
			break;
		}

		phaseIncrement = frequency * samplePeriod;
		currentPhase += phaseIncrement;
		currentPhase -= static_cast<int>(currentPhase);

		return sampleValue;
	}

private:
	int waveform;
	float frequency;

	double currentPhase = 0;
	double phaseIncrement = 0;
	float samplePeriod = 1.0;

	
};

class PhaseDistortedOsc
{
public:
	PhaseDistortedOsc( float defaultFrequency = 300.0f, float defaultParameter = 0.0f, int defaultFunction = 0)
	
	{
    frequency = defaultFrequency;
    parameter = defaultParameter;
		function = defaultFunction;

		//this->connectTo(processor.parameterEnvelope);
	}

	~PhaseDistortedOsc() {}

	void prepareToPlay(const float sr)
	{

		samplePeriod = 1.0 / sr;
	}

	void setFrequency(float newValue)
	{
		frequency = newValue;
	}

	void setParameter(float newValue)
	{
		parameter = newValue;
	}

  void setFunc1(int newValue)
  {
    func1 = newValue; 
  }
  void setFunc2(int newValue)
  {
    func2 = newValue;
  }

  void setWindow(int newValue)
  {
    window = newValue;
  }

	void getNextAudioBlock(float** buffer, const int numSamples)
	{
		const int numCh = 2; //buffer.getNumChannels();
		

		for (int s = 0; s < numSamples; ++s)
		{
			const float sampleValue = getNextAudioSample();


			for (int ch = 0; ch < numCh; ++ch)
			{
		      buffer[ch][s] = sampleValue;
			}
		}
	}

	float getNextAudioSample()
	{
		auto sampleValue = 0.0;
		//auto ps = 0.0;
		float ps = parameter;
		
		double y = 0.5;
		switch (function)
		{
		case 0: // SAW
		{
			ps = (1 - ps) * 0.5;			
			if (currentPhase < ps)
			{
				double m0a = y / ps;
				distortedPhase = m0a * currentPhase;
			}
			else 
			{
				double m0b = y / (1 - ps);
				double b0 = 1 - m0b;
				distortedPhase = m0b * currentPhase + b0;
			}
			sampleValue = cos(TWO_PI * distortedPhase);
			//sampleValue = distortedPhase;
			break;
		}
			
	
		case 1: // UNI PULSE
		{
			ps *= 0.98;
			ps = ps * 0.5;
			double m = 0.5 / (0.5 - ps);
			double b = 0.5 - m * 0.5;

			double x = -b / m;

			if (currentPhase < x)
			{
				distortedPhase = 0;
			}
			else if (currentPhase < 1 - x)
			{
				distortedPhase = m * currentPhase + b;
			}
			else
			{
				distortedPhase = 1;
			}
			sampleValue = cos(TWO_PI * distortedPhase);
		}
			break;
		
		case 2: // SIN-PULSE (giusto)
		{
			ps *= 0.5;
			double m1 = 1/(0.5+ps);
			double b1 = 0;
			double m2 = 1 / (0.5 - ps);
			double b2 = 1 - m2;

			double x1 = -b2/ m2;


			if (currentPhase < x1)
			{
				distortedPhase = m1* currentPhase +b1;
			}
			else
			{
				distortedPhase = m2 *  currentPhase + b2;
			}
			sampleValue = cos(TWO_PI * distortedPhase);
		}
			break;
		case 3: // correct SQUARE
		{
			ps = 1 - ps;
			ps *= 0.5;
			double m1 = 0.5 / ps;
			double b1 = 0.5 - (m1 * 0.5);

			double b2 = 1 - m1;
			double x1 = -b1 / m1;

			if (currentPhase < x1)
			{
				distortedPhase = 0;
			}
			else if (currentPhase < 0.5)
			{
				distortedPhase = m1 * currentPhase + b1;
			}
			else if (currentPhase < x1 + 0.5)
			{
				distortedPhase = 0.5;
			}
			else
			{
				distortedPhase = m1 * currentPhase + b2;
			}
			sampleValue = cos(TWO_PI * distortedPhase);
		}
			break;
		default:
			break;
		}

    float win = 0.0;

    switch (window)
    {
      case 0:
      {
        win = 1- currentPhase;
      }
      break;
      case 1:
      {
        win = 1;
      }
      break;
      case 2:
        win = 1;
      break;
      default:
        win = 1;
      break;
    }

    sampleValue *= win;

		phaseIncrement = frequency * samplePeriod;
		currentPhase += phaseIncrement;

    if (currentPhase >= 1.0)
    {
      if (turn)
      {
        function = func1;
      }
      else
      {
        function = func2;
      }

      turn != turn;
    }
		currentPhase -= static_cast<int>(currentPhase);
	
		return sampleValue;
		

	}



private:
  bool turn =  true;
	int function = 0;
  int func1 = 0;
  int func2 = 0;


  int window = 0;
	float frequency;
	float parameter;


	// delta = w0/fs
	double phaseIncrement = 0;


	//tra 0 e 1
	double currentPhase = 0;
	double samplePeriod = 1.0;
	double distortedPhase = 0;

};

