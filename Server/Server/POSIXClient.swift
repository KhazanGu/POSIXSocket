//
//  POSIXClient.swift
//  Server
//
//  Created by Khazan on 2023/8/30.
//

import Foundation

protocol POSIXClientDelegate {
    
    func didReceiveMessage(with string: String) -> Void
    
    func didDisconnect(client: POSIXClient) -> Void
    
}

class POSIXClient: NSObject {
    
    func connectServer(with ipAddress: String, port: Int) -> Void {
        guard let socket = socket else { return }
        
        ipAddress.withCString { pointer in
            let status = client_socket_connect_server_socket(socket, pointer, Int16(port))
            
            if status == 0 {
                client_socket_receive_message_on_child_thread(socket) {
                    
                } _: { pointer in
                    if let pointer = pointer {
                        let string = String(cString: pointer)
                        self.delegate?.didReceiveMessage(with: string)
                    }
                }
            }
        }
        
    }
    
    func send(string: String) -> Void {
        guard let socket = socket else { return }
        
        string.withCString { pointer in
            client_socket_send_message(socket, pointer)
        }

    }
    
    func receiveMessage() -> Void {
        guard let socket = socket else { return }

        client_socket_receive_message_on_child_thread(socket) {
            self.delegate?.didDisconnect(client: self)
        } _: { pointer in
            if let pointer = pointer {
                let string = String(cString: pointer)
                self.delegate?.didReceiveMessage(with: string)
            }
        }

    }
    
    init(with socket: Int32) {
        self.socket = socket
    }
    
    override init() {
        super.init()
        
        self.socket = client_socket_create()
    }
    
    var socket: Int32?
    
    var delegate: POSIXClientDelegate?
    
}



