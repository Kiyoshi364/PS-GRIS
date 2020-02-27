package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net"
	"net/http"
	"sync"
	"time"
)

type MyHandler struct{}

type Target struct {
	host	string	`json:"host"`
	start	int		`json:"start"`
	end		int		`json:"end"`
}

func (h *MyHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Scanning ports...")
	log.Printf("New request: %v", r)

	// Parse JSON
	t := &Target{}
	decoder := json.NewDecoder(r.Body)
	err := decoder.Decode(t)
	if  err != nil {
		//http.Error(w, err.Error(), http.StatusInternalServerError)
		//return
	}

	fmt.Fprintf(w, "\tTarget: %v", t)

	/* Constantes para teste DEBUG
	*/
	t.host = "192.168.0.38"//"scanme.nmap.org"//"127.0.0.1"
	t.start = 0010
	t.end = 8090
	/*
	*/

	// Channel para retornar os ports abertos
	openPort := make(chan int)

	// WaitGroup para esperar as goroutines acabarem
	var wg sync.WaitGroup

	// Inicializar goroutines
	for port := t.start; port <= t.end; port++ {
		wg.Add(1)
		go scan(t.host, port, &wg, openPort)
	}

	log.Printf("Before Wait") // DEBUG
	wg.Wait()
	log.Printf("After Wait") // DEGUB
	close(openPort)

	// Printar ports abertos (possivelmente ordenados)
	for port := range openPort {
		fmt.Fprintf(w, "\nOpen port detected: %d", port)
	}
	log.Printf("Ended Request")
}

func scan(host string, port int, wg *sync.WaitGroup, openPort chan int) error {
	defer wg.Done()

	// Formatar endereço
	address := fmt.Sprintf("%s:%d", host, port)

	// Abrir coneção
	conn, err := net.Dial("tcp", address)

	// Se tem erro, esta fechado
	if err != nil {
		//log.Printf("\tPort %d: closed", port) // DEBUG
		return err
	} else {
		conn.Close()
		openPort <- port
		log.Printf("\tPort %d: open", port) // DEBUG
		return nil
	}
}

func main() {
	server := &http.Server {
		Addr:			"127.0.0.1:8081",
		Handler:		&MyHandler{},
		ReadTimeout:	10 * time.Second,
		WriteTimeout:	10 * time.Second,
		IdleTimeout:	time.Second,
	}

	log.Printf("Running web server on https://%s\n", server.Addr)
	log.Fatal(server.ListenAndServe())
}
