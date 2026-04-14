const Papersub = () => {
  return (
    <div className="site-card w-full max-w-4xl mx-auto my-8 py-16">
      <section className="p-8">
        <h2 className="text-3xl font-semibold mb-4 text-zinc-900 dark:text-slate-50">Paper Submission</h2>
        <p className="text-zinc-700 dark:text-slate-200 text-lg mb-6">How and where to submit your research paper.</p>
        <ul className="list-disc pl-6 text-zinc-700 dark:text-slate-200 text-lg">
          {/* <li><strong className="font-semibold">Submission Starts:</strong> June 16</li> */}
          <li><strong className="font-semibold">Paper Submission Deadline:</strong><span className='underline bg-amber-200 px-1 rounded dark:bg-amber-500/20 dark:text-amber-100'>October 25, 2026</span></li>
          <li><strong className="font-semibold">Notification Acceptance:</strong> 25 January, 2027</li>
          <li><strong className="font-semibold">Date of Conference:</strong>March 5th - 7th, 2027</li>
          <li><strong className="font-semibold">Early Bird Registration:</strong>10 January, 2027</li>
          <li><strong className="font-semibold">Camera-Ready Submission:</strong>24 January, 2027</li>
          <li>
            <strong className="font-semibold">Submission link:</strong> 
            <span className='hover:cursor-pointer underline font-semibold text-indigo-700 dark:text-indigo-200' onClick={()=>window.open("https://cmt3.research.microsoft.com/ICNARI2027","_blank")}> Microsoft CMT</span>
          </li>
        </ul>
      </section>
    </div>
  )
}

export default Papersub;
