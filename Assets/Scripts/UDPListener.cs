using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
// Here are the libraries required for the UDP listener. 
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System;


public class UDPListener : MonoBehaviour
{
    //public TextMeshProUGUI Notifications;
    private const int S1Port= 65013;
    private const int S2Port = 6514;
    private const int S3Port = 65015;
    private const int P1Port = 65016;
    private const int P2Port = 65017;
    private const int R1Port = 65018;

    public TMP_Text Switch1Text;
    public TMP_Text Switch2Text;
    public TMP_Text Switch3Text;
    public TMP_Text Pot1Text;
    public TMP_Text Pot2Text;
    public TMP_Text Rot1Text;


    private int RXPort;
    private TMP_Text DisplayText;

    Thread receiveThread;

    private string DisplayMessage;
    public Action NewText;

    public string DataIn;
    // Start is called before the first frame update
    void Start()
    {
        receiveThread = new Thread(
           new ThreadStart(ReceiveData));
        receiveThread.IsBackground = true;
        receiveThread.Start();

        //myDelegate = LeftArrow;

        // assign our delegates to specific functions.
        NewText = ChangeText;

        // Instantiate an instance of our dispatcher class.
        Dispatcher _instance = new Dispatcher();


    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void ReceiveData()
    {
        Debug.Log("Starting to recieve data");
        UdpClient S1client = new UdpClient(S1Port);
        while (true)
        {

            try
            {

                RXPort = S1Port;
                DisplayText = Switch1Text;
                IPEndPoint anyIP = new IPEndPoint(IPAddress.Any, 0);
                byte[] data = S1client.Receive(ref anyIP);

                DataIn = Encoding.UTF8.GetString(data);

                print(DataIn);

                DisplayMessage = DataIn;
                Dispatcher.RunOnMainThread(NewText);
                
            }
            catch (SocketException err)
            {
                print(err.ToString());
            }
        }

    }

    private void ChangeText()
    {
        DisplayText.text = RXPort.ToString() + DisplayMessage;
    }
}
