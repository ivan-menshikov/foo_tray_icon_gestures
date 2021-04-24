param($LiteralPath, $Destination)

function createZipFromFile($fileName, $zipName)
{
    if (Test-Path -LiteralPath $zipName)
    {
        Remove-Item -LiteralPath $zipName -ErrorAction Stop
    }

    Add-Type -AssemblyName 'System.IO.Compression' -ErrorAction Stop
    Add-Type -AssemblyName 'System.IO.Compression.FileSystem' -ErrorAction Stop

    $entryName = Split-Path -Leaf $fileName

    $fileStream = [System.IO.File]::OpenRead($fileName)
    try
    {
        $zipMode = [System.IO.Compression.ZipArchiveMode]::Create
        $zip = [System.IO.Compression.ZipFile]::Open($zipName, $zipMode)
        try
        {
            $entry = $zip.CreateEntry($entryName)
            $entryStream = $entry.Open()
            try
            {
                $fileStream.CopyTo($entryStream)
            }
            finally
            {
                $entryStream.Dispose()
            }
        }
        finally
        {
            $zip.Dispose()
        }
    }
    finally
    {
        $fileStream.Dispose()
    }
}

$ErrorActionPreference = 'Stop'
createZipFromFile $LiteralPath $Destination
'Created: {0}' -f $Destination | Write-Host
