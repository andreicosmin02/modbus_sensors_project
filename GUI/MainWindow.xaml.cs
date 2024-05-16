using System.Text;
using System.IO.Ports;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using LiveCharts;
using LiveCharts.Wpf;
using System.Windows.Threading;

namespace ModbusSensorsApp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private bool isConnected = false;
        private string port = "";
        private int baudRate = 0;
        private ModbusMessage modbusMessage = new ModbusMessage();

        private SeriesCollection _seriesCollection;
        private Queue<double> _temperatureValues = new Queue<double>();
        private Queue<double> _humidityValues = new Queue<double>();

        public SeriesCollection SeriesCollection
        {
            get { return _seriesCollection; }
            set { _seriesCollection = value; }
        }


        public MainWindow()
        {
            InitializeComponent();

            // Inițializarea graficului
            _seriesCollection = new SeriesCollection
        {
            new LineSeries
            {
                Title = "Temperature",
                Values = new ChartValues<double>()
            },
            new LineSeries
            {
                Title = "Humidity",
                Values = new ChartValues<double>()
            }
        };

            DataContext = this;

            // Programarea funcției pentru a actualiza graficul la fiecare 5 secunde
            DispatcherTimer timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(5);
            timer.Tick += Timer_Tick;
            timer.Start();
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            if (isConnected)
            {
                try
                {
                    // Citirea temperaturii și umidității
                    (ushort humidity, ushort temperature) = modbusMessage.ReadTemperatureAndHumidity();

                    // Actualizarea cozilor de valori
                    _temperatureValues.Enqueue(temperature);
                    _humidityValues.Enqueue(humidity);

                    // Limitarea cozilor la 10 valori
                    if (_temperatureValues.Count > 10)
                        _temperatureValues.Dequeue();
                    if (_humidityValues.Count > 10)
                        _humidityValues.Dequeue();

                    // Actualizarea graficului
                    UpdateChart();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error reading temperature and humidity:\n" + ex.Message);
                    Disconnect();
                }
            }
        }
        private void UpdateChart()
        {
            // Ștergerea datelor vechi din grafic
            _seriesCollection[0].Values.Clear();
            _seriesCollection[1].Values.Clear();

            // Adăugarea noilor valori în grafic
            foreach (double temperature in _temperatureValues)
            {
                _seriesCollection[0].Values.Add(temperature);
            }

            foreach (double humidity in _humidityValues)
            {
                _seriesCollection[1].Values.Add(humidity);
            }
        }


        private void ExitButtonClick(object sender, MouseButtonEventArgs e)
        {

            System.Windows.Application.Current.Shutdown();
        }

        private void DragWindow(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                this.DragMove();
        }

        private void Disconnect()
        {
            modbusMessage.Disconnect();
            ConnectTextBlock.Text = "Connect";
            isConnected = false;
        }

        private void ConnectButton_Click(object sender, RoutedEventArgs e)
        {
            if (!isConnected)
            {
                try
                {
                    string port = PortBox.Text;
                    int baudRate = int.Parse(BaudRateBox.Text);

                    if (modbusMessage.Connect(port, baudRate))
                    {
                        ConnectTextBlock.Text = "Disconnect";
                        isConnected = true;

                        // Încărcare starea LED-urilor la conectare
                        try
                        {
                            bool[] ledStates = modbusMessage.LoadLEDs();
                            Led1Checkbox.IsChecked = ledStates[0];
                            Led2Checkbox.IsChecked = ledStates[1];
                            Led3Checkbox.IsChecked = ledStates[2];
                            Led4Checkbox.IsChecked = ledStates[3];
                            Led5Checkbox.IsChecked = ledStates[4];
                        }
                        catch (Exception ex)
                        {
                            Disconnect();
                            MessageBox.Show("Error loading LED states: " + ex.Message);
                        }
                    }
                    else
                    {
                        Disconnect();
                        MessageBox.Show("Failed to connect to port " + port);
                    }
                }
                catch (Exception ex)
                {
                    Disconnect();
                    MessageBox.Show("Error: " + ex.Message);
                }
            }
            else
            {
                Disconnect();
            }
        }


        private void DataReceivedHandler(
                        object sender,
                        SerialDataReceivedEventArgs e)
        {

        }

        private void DistanceButton_Click(object sender, RoutedEventArgs e)
        {
            if (!isConnected)
            {
                MessageBox.Show("Device is not connected");
                return;
            }

            int distance = 0;
            try
            {
                distance = modbusMessage.CalculateDistance();
            }
            catch (Exception ex ) 
            {
                MessageBox.Show("Error on reading data\n" + ex.Message);
                Disconnect();
            }

            DistanceTextBlock.Text = $"{distance} cm";
        }


        private void Led1Checkbox_Click(object sender, RoutedEventArgs e)
        {
            if (!isConnected)
            {
                MessageBox.Show("Device is not connected");
                return;
            }

            try
            {
                if (Led1Checkbox.IsChecked == true)
                {
                    modbusMessage.SetLEDState(0, false);
                }
                else
                {
                    modbusMessage.SetLEDState(0, true);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error setting LED state:\n" + ex.Message);
                Disconnect();
            }
        }

        private void Led2Checkbox_Click(object sender, RoutedEventArgs e)
        {
            if (!isConnected)
            {
                MessageBox.Show("Device is not connected");
                return;
            }

            try
            {
                if (Led2Checkbox.IsChecked == true)
                {
                    Disconnect();
                    modbusMessage.SetLEDState(1, false);
                }
                else
                {
                    Disconnect();
                    modbusMessage.SetLEDState(1, true);
                }
            }
            catch (Exception ex)
            {
                Disconnect();
                MessageBox.Show("Error setting LED state:\n" + ex.Message);
            }
        }


        private void Led3Checkbox_Click(object sender, RoutedEventArgs e)
        {
            if (!isConnected)
            {
                MessageBox.Show("Device is not connected");
                return;
            }

            try
            {
                if (Led3Checkbox.IsChecked == true)
                {
                    Disconnect();
                    modbusMessage.SetLEDState(2, false);
                }
                else
                {
                    Disconnect();
                    modbusMessage.SetLEDState(2, true);
                }
            }
            catch (Exception ex)
            {
                Disconnect();
                MessageBox.Show("Error setting LED state:\n" + ex.Message);
            }
        }

        private void Led4Checkbox_Click(object sender, RoutedEventArgs e)
        {
            if (!isConnected)
            {
                MessageBox.Show("Device is not connected");
                return;
            }

            try
            {
                if (Led4Checkbox.IsChecked == true)
                {
                    modbusMessage.SetLEDState(3, false);
                }
                else
                {
                    modbusMessage.SetLEDState(3, true);
                }
            }
            catch (Exception ex)
            {
                Disconnect();
                MessageBox.Show("Error setting LED state:\n" + ex.Message);
            }
        }


        private void Led5Checkbox_Click(object sender, RoutedEventArgs e)
        {
            if (!isConnected)
            {
                MessageBox.Show("Device is not connected");
                return;
            }

            try
            {
                if (Led5Checkbox.IsChecked == true)
                {
                    modbusMessage.SetLEDState(4, false);
                }
                else
                {
                    modbusMessage.SetLEDState(4, true);
                }
            }
            catch (Exception ex)
            {
                Disconnect();
                MessageBox.Show("Error setting LED state:\n" + ex.Message);
            }
        }
    }
}