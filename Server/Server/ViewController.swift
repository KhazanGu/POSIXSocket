//
//  ViewController.swift
//  Server
//
//  Created by Khazan on 2023/8/30.
//

import Cocoa

class ViewController: NSViewController {

    
    @IBAction func send(_ sender: NSButton) {
        
        self.clients.map { client in
            
            let string = self.sendTextField.stringValue
            
            client.send(string: string)
        }
    }
    
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        
        server.delegate = self
        
        server.run()
        
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }

    @IBOutlet weak var numberLabel: NSTextField!
    
    @IBOutlet weak var sendTextField: NSTextField!
    
    @IBOutlet var receiveTextView: NSTextView!
    
    var server = POSIXServer()

    var clients = [POSIXClient]()
}


extension ViewController: POSIXServerDelegate {
    
    func didConnect(with client: POSIXClient) {
        client.delegate = self
        
        client.receiveMessage()
        
        clients.append(client)
        
        DispatchQueue.main.async {
            self.numberLabel.stringValue = "Number: \(self.clients.count)"
        }
    }
    
}

extension ViewController: POSIXClientDelegate {
    
    func didReceiveMessage(with string: String) {
        
        DispatchQueue.main.async {
            self.receiveTextView.string = self.receiveTextView.string + string
        }
        
    }
    
    func didDisconnect(client: POSIXClient) {
        self.clients.removeAll()
        DispatchQueue.main.async {
            self.numberLabel.stringValue = "Number: \(self.clients.count)"
        }
    }
    
}
