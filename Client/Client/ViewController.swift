//
//  ViewController.swift
//  Client
//
//  Created by Khazan on 2023/8/30.
//

import UIKit

class ViewController: UIViewController {

    
    @IBAction func send(_ sender: Any) {
        
        if let text = self.sendTextField.text {
            client.send(string: text)
        }
        
    }
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        
        client.delegate = self
        
        client.connectServer(with: ip, port: port)
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        self.view.endEditing(true)
    }

    var client = POSIXClient()

    var ip = "192.168.21.16"
    
    var port = 8888
    
    @IBOutlet weak var statusLabel: UILabel!
    
    @IBOutlet weak var sendTextField: UITextField!
    
    @IBOutlet weak var receiveTextView: UITextView!

}


extension ViewController: POSIXClientDelegate {
    
    func didReceiveMessage(with string: String) {
        
        print("didReceiveMessage: \(string)")
        
        DispatchQueue.main.async {
            self.receiveTextView.text = self.receiveTextView.text + string
        }
    }
    
    func didConnect(client: POSIXClient) {
        DispatchQueue.main.async {
            self.statusLabel.text = "Status: On"
        }
    }
    
    func didDisconnect(client: POSIXClient) {
        DispatchQueue.main.async {
            self.statusLabel.text = "Status: Off"
        }
    }
    
}


