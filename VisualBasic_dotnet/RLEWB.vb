Public Class RLEWB

    Private Const RLEWB_CONTROL As Byte = 128
    Private Const RLEWB_END As Byte = &HFF

   
    ''' <summary>
    ''' Run-Length Encoding (RLE WB)
    ''' Inspired from the Wonder Boy RLE compression algorithm, published on the SMS POWER! WEBSITE.
    ''' http://www.smspower.org/Development/Compression#WonderBoyRLE
    ''' $80 nn dd            ; run of n consecutive identical bytes ($1>$FE), value dd
    ''' $80 $80              ; zero value
    ''' $80 $FF              ; end of data block
    ''' any other value      ; raw data  
    ''' </summary>
    ''' <param name="data"></param>
    ''' <returns></returns>
    ''' <remarks></remarks>
    Public Function GetRLEWB(ByVal data() As Byte) As Byte()
        Dim RLEtmpData As New ArrayList
        Dim RLEtmpRaw As New ArrayList
        Dim RLEoutputData() As Byte
        Dim isEnd As Boolean = False

        Dim num_rep As Byte = 0
        Dim num_raw As Byte = 0
        Dim code_pos As Integer = 0

        Dim position As Integer = 0
        Dim position2 As Integer = 0
        Dim value As Byte
        'Dim nextValue As Byte
        'Dim valueRAW As Boolean

        Do
            value = data(position)
            position2 = position + 1


            Do While position2 < data.Length AndAlso value = data(position2) And num_rep < 254
                num_rep += 1
                position2 += 1
            Loop

            If num_rep > 1 Then
                'only when they are more than 2 repetitions
                'two repeats are considered Raw. This way, 3 bytes are not wasted for two values.
                RLEtmpData.Add(RLEWB_CONTROL)
                RLEtmpData.Add(num_rep)
                RLEtmpData.Add(value)

                position = position2
            Else
                ' write raw value
                If value = RLEWB_CONTROL Then
                    ' $80 , 0
                    RLEtmpData.Add(RLEWB_CONTROL)
                    RLEtmpData.Add(0)
                Else
                    ' raw
                    RLEtmpData.Add(value)
                End If
                position += 1
            End If

            num_rep = 0

        Loop While (position < data.Length)

        RLEtmpData.Add(RLEWB_CONTROL)
        RLEtmpData.Add(RLEWB_END) ' marca de final

        ReDim RLEoutputData(RLEtmpData.Count - 1)

        For i As Integer = 0 To RLEtmpData.Count - 1
            RLEoutputData(i) = RLEtmpData.Item(i)
        Next

        Return RLEoutputData

    End Function


End Class
