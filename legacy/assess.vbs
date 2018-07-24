Option Explicit
Dim rqsdURL
Dim rqsdTme
Dim t
rqsdURL = InputBox("Report Domains", "Submit")

Dim IE
Set IE = CreateObject("InternetExplorer.Application")
IE.Visible = True
IE.Navigate rqsdURL


While IE.readyState <> 1
Wend
t = Timer

While IE.readyState <> 3
Wend

rqsdTme = Timer - t
MsgBox(rqsdTme)

Dim ref, entry
Set ref = IE.document.getElementsByTagName("link")
MsgBox(IE.document)

    For Each entry In ref
        'MsgBox(entry.href)
    Next

IE.Quit

Function Assessment(stdRqsdURL)
    
    Dim IE, rqsdTme, t
    Set IE = CreateObject("InternetExplorer.Application")
    IE.Visible = False
    IE.Navigate stdRqsdURL

    While IE.readyState <> 1
    Wend
    t = Timer

    While IE.readyState <> 3
    Wend

    rqsdTme = Timer - t
    Return rqsdTme

End Function